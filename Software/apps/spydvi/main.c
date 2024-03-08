// #pragma GCC optimize("Os")
// #pragma GCC optimize("O2")
//#pragma GCC optimize("O3")

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/uart.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"

#include "n64.pio.h" // n64.pio contains pin definitions for SpyDVI N64 Header

#include "testcard_320x240_rgb565.h"

// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

// UART DEF
#define UART_ID uart0
#define BAUD_RATE 115200

// Define DEBUG pins
#define UART_TX_PIN 16
#define UART_RX_PIN 17


const PIO pio = pio1; //DVI_DEFAULT_SERIAL_CONFIG.pio; // Default pio0
const uint sm_video = 0; // State machine ID?
const uint sm_audio = 1; // State machine ID?

struct dvi_inst dvi0;

void set_input_pin(int pin, bool pullup, bool pulldown)
{
	gpio_init(pin);
	gpio_set_dir(pin, GPIO_IN);

	// Enable weak internal pull downs to reduce noise when n64 is turned off
    gpio_set_pulls(pin, pullup, pulldown);
}

void core1_main() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	while (queue_is_empty(&dvi0.q_colour_valid))
		__wfe();
	dvi_start(&dvi0);
	dvi_scanbuf_main_16bpp(&dvi0);
}

int main() {
	
	// Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

	// Send mesage on UART to indicate it is working
    uart_puts(UART_ID, " Hello, UART!\n");
	uart_puts(UART_ID, "There could be a spy in this very room!");
	
	vreg_set_voltage(VREG_VSEL);
	sleep_ms(10);
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

	bool setAVPulldown = true;
	set_input_pin(n64_VIDEO_D0, false, setAVPulldown);
	set_input_pin(n64_VIDEO_D1, false, setAVPulldown);
	set_input_pin(n64_VIDEO_D2, false, setAVPulldown);
	set_input_pin(n64_VIDEO_D3, false, setAVPulldown);
	set_input_pin(n64_VIDEO_D4, false, setAVPulldown);
	set_input_pin(n64_VIDEO_D5, false, setAVPulldown);
	set_input_pin(n64_VIDEO_D6, false, setAVPulldown);
	set_input_pin(n64_VIDEO_DSYNC, false, setAVPulldown);
	set_input_pin(n64_VIDEO_CLK, false, setAVPulldown);
	set_input_pin(n64_AUDIO_LRCLK, false, setAVPulldown);
	set_input_pin(n64_AUDIO_SDAT, false, setAVPulldown);
	set_input_pin(n64_AUDIO_BCLK, false, setAVPulldown);

	set_input_pin(n64_JOYBUS_CON1, false, false); // Controller Pin

	// Core 1 will wait until it sees the first colour buffer, then start up the
	// DVI signalling.
	multicore_launch_core1(core1_main);

	// Init N64 Video PIO
    uint offset = pio_add_program(pio, &n64_program);
    n64_video_program_init(pio, sm_video, offset);
    pio_sm_set_enabled(pio, sm_video, true);

	// Init N64 Audio PIO
    n64_audio_program_init(pio, sm_audio, offset);
    pio_sm_set_enabled(pio, sm_audio, true);

	// Pass out pointers into our preprepared image, discard the pointers when
	// returned to us. Use frame_ctr to scroll the image
	uint frame_ctr = 0;
	while (true) {
		for (uint y = 0; y < FRAME_HEIGHT; ++y) {
			uint y_scroll = (y + frame_ctr) % FRAME_HEIGHT;
			const uint16_t *scanline = &((const uint16_t*)testcard_320x240)[y_scroll * FRAME_WIDTH];
			queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
			while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline))
				;
		}
		++frame_ctr;
	}
}