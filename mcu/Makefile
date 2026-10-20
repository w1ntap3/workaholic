PROGRAMMER := ~/.local/share/stm32cube/bundles/programmer/2.23.0/bin/STM32_Programmer_CLI
TERM := /dev/ttyACM0

r: # (r)elease
	cmake --build build/Release
	${PROGRAMMER} -c port=SWD -w build/Release/nucleo.elf -v -rst

w: # (w)atch
	picocom -b 115200 ${TERM}

