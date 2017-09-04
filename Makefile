all: minesweeper.hex

minesweeper.elf: *.c *.h
	avr-gcc -mmcu=atmega324p -DF_CPU=16000000 -std=c99 -Wall -Os -o $@ $^

minesweeper.hex: minesweeper.elf
	avr-objcopy -j .text -j .data -O ihex minesweeper.elf minesweeper.hex
	avr-size minesweeper.elf

clean:
	rm -rf minesweeper.elf minesweeper.hex

.PHONY: all clean
