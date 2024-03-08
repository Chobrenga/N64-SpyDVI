# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

add_executable(spydvi
	main.c
)

target_compile_options(spydvi PRIVATE -Wall)

target_compile_definitions(spydvi PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	)

target_link_libraries(spydvi
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
)

#build PIO
pico_generate_pio_header(spydvi ${CMAKE_CURRENT_LIST_DIR}/n64.pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(spydvi)
