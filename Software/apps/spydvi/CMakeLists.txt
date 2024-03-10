# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

add_executable(spydvi
	config.c
	gfx.c
	joybus.c
	main.c
	osd.c
)

target_compile_options(spydvi PRIVATE -Wall)

target_compile_definitions(spydvi PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	CONFIG_DEFAULT_SAMPLE_RATE_HZ=${CONFIG_DEFAULT_SAMPLE_RATE_HZ}
	CONFIG_DEFAULT_COLOR_DEPTH=${CONFIG_DEFAULT_COLOR_DEPTH}
	)

target_link_libraries(spydvi
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	libsprite
	hardware_pio
)

#build PIO
pico_generate_pio_header(spydvi ${CMAKE_CURRENT_LIST_DIR}/joybus.pio)
pico_generate_pio_header(spydvi ${CMAKE_CURRENT_LIST_DIR}/n64.pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(spydvi)
