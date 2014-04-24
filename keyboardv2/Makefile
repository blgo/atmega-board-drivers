# Program
PRG=keyboardv2
OBJ=main.o
LIBS=

# Board and programmer
PORT=usb
PROGRAMMER=usbasp
MCU_TARGET=atmega1280
AVRDUDE_TARGET=atmega1280

# Compiling
CC=avr-gcc

OPTIMIZE=-Os 
CFLAGS=-g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET)
LDFLAGS=-Wl,-Map,$(PRG).map

OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRDUDE=avrdude

all: $(PRG).elf hex install

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

lst: $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

text: hex bin srec
 
hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec
 
%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
 
%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@
 
%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@
 
# Rules for building the .eeprom rom images
 
eeprom: ehex ebin esrec
 
 
ehex:  $(PRG)_eeprom.hex
#ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec
 
%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@
 
#%_eeprom.srec: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@
 
%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@
 
 
# command to program chip (invoked by running "make install")
install:  $(PRG).hex
	$(AVRDUDE) -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v  \
         -U flash:w:$(PRG).hex 
 
fuse:
	$(AVRDUDE) -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
	-U lfuse:w:0xc6:m -U hfuse:w:0xd9:m 	

ping:
	$(AVRDUDE) -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) 

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec
	rm -rf *.lst *.map
