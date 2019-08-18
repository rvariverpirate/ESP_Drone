#! /bin/bash/

echo -e "Enter name of .c file to to compile and upload: "
read Filename
cd $Filename
echo "Creating $Filename.o file..."
avr-gcc -g -Os -mmcu=atmega328p -c $Filename.c
echo "Creating $Filename.elf file..."
avr-gcc -g -mmcu=atmega328p -o $Filename.elf $Filename.o
echo "Creating $Filename.hex file..."
avr-objcopy -j .text -j .data -O ihex $Filename.elf $Filename.hex
echo "Flashing $Filename.hex to MCU..."
avrdude -c usbasp -p atmega328p -U flash:w:$Filename.hex -v -F
