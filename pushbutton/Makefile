
#Makefile example 
#----------first declare variables(all strings/text) used later -------

#set path(need to do this even if its already in the search path)
#(if dont know path then write in linux shell: which avr-gcc)
AVRGCCDIR="/home/jessica/arduino-install/arduino-1.8.19/hardware/tools/avr"

#select chip
#difference between MCU and DUDECPUTYPE: one is used by avr-gcc the other by avrdude(here they are same but for other chips this might not be the case)
MCU=atmega328p
DUDECPUTYPE=atmega328p

#select port
#for nano:
  #for linux:
#DUDEPORT=/dev/ttyUSB0
# for uno:
  #for linux:
DUDEPORT=/dev/ttyACM0
  #for windows(the nb might change):
#DUDEPORT=COM7

#set compiler and things like that
PROGRAMER=arduino
LOADCMD=$(AVRGCCDIR)/bin/avrdude
CC=$(AVRGCCDIR)/bin/avr-gcc
OBJCOPY=$(AVRGCCDIR)/bin/avr-objcopy

DUDECONF=$(AVRGCCDIR)/etc/avrdude.conf

#set speed of data transmission(if have wrong speed then cant talk to chip)
# for some nanos its 57600 while for uno and some nanos its 11560 which is the default
#BAUTRATE=-b57600
#(for uno board) its default so no need to set it
#the -b is put here instead of in avrdude because if you dont set baudrate then you give avrdude nothing rather then -b nothing. and avrdude will complain if it gets -b nothing

#set a varibale called LOADARG with other variables($ means its a variable)
LOADARG=-C $(DUDECONF) -P $(DUDEPORT) $(BAUTRATE) -p $(DUDECPUTYPE) -c $(PROGRAMER) -e -U flash:w:

#set variable called CFLAGS
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

#-------------
#this specifies what the make or make all shell command should do
#you tell it what file you want as the default
all: button.hex
#--------------
#this specifies what the make help shell command should do
#echo is a command that prints text to screen, the @ hides the command so that you dont see echo text
help: 
	@echo "Usage: make all|load|rdfuses|clean"
	@echo "  make or make all: compile "
	@echo "  make load: use avrdude to load the code "
	@echo "  make rdfuses: use avrdude to read fuses and test connection"
	@echo "  make clean: delete all generated compilation files"
#-------------------
#here we say how to make the file we specified in all
#here we say if we want .hex then we need the .out file and if we want .out we need .o and to get .o we need .c 
button.hex : button.out 
	$(AVRGCCDIR)/bin/avr-size button.out
	$(OBJCOPY) -R .eeprom -O ihex button.out button.hex 
button.out : button.o 
	$(CC) $(CFLAGS) -o button.out -Wl,-Map,button.map button.o 
button.o : button.c 
	$(CC) $(CFLAGS) -Os -c button.c

#---------------
#this specifies what the make load shell command should do
load: button.hex
	$(LOADCMD) $(LOADARG)button.hex

#-------------------
#this specifies what the make rdfuses shell command should do
rdfuses:
	$(AVRGCCDIR)/bin/avrdude -C $(DUDECONF) -P $(DUDEPORT) $(BAUTRATE) -p $(DUDECPUTYPE) -c $(PROGRAMER) -v -q
#-------------------
#this specifies what the make clean shell command should do
clean:
#for linux:
	rm -f *.o *.map *.out *.hex
#for windows:
#	del *.o *.map *.out *.hex
#-------------------


