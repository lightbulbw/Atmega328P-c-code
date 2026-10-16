 
//this file is called blinkled.c but you can name it what you want as long as the makefile contains the same name
#include <avr/io.h>//this library is needed to use IO pins(input/output pins)

//_delay_ms(x) fct uses this to know how many cycles a second is
//_delay_ms(x) does not use prescaler
//for uno and nano use 16MHz
#define F_CPU 16000000UL  // 16 MHz

//this library contains _delay_ms(x);
#include <util/delay.h>//this line needs to be after #define F_CPU because delay needs F_CPU

//led on nano or uno board goes to ground(so 1 is on)
#define LEDOUT PORTB5 //pin (bit)
#define LEDPORT PORTB //port(this is a register)
#define LEDDDR DDRB   //register needed to set pin to input or output
#define LEDDDRPIN DDB5  //register digit(aka bit) needed to set pin to input or output

int main(void){
	//enable pin as output 
	LEDDDR|= (1<<LEDDDRPIN);//if instead wanted to set to input do: LEDDDR&= ~(1<<LEDDDRPIN);
	while (1) {
		//this fct is a busy wait delay
		_delay_ms(500);//this fct is in C library included at top
		//led off 
		LEDPORT &= ~(1<<LEDOUT);//write 0 for off (because negation(~) we do &)(doing LEDPORT&=(0<<LEDOUT); does the same thing
	
		_delay_ms(500);
	
		//led on
		LEDPORT|= (1<<LEDOUT); //write 1 for on(because  not negation(~) we do |)

	}

 return(0);
}
