//this code turns on an LED(on pin 13) while a push button(on D2) is pressed.
#include <avr/io.h>//this library is needed to use IO pins(input/output pins)

//_delay_ms(x) fct uses this to know how many cycles a second is
//_delay_ms(x) does not use prescaler
//for uno and nano use 16MHz
#define F_CPU 16000000UL  // 16 MHz

//this library contains _delay_ms(x);
#include <util/delay.h>//this line needs to be after #define F_CPU because delay needs F_CPU

//led on nano or uno board(B5 aka D13) goes to ground(so 1 is on)
#define LEDOUT PORTB5 //pin (bit)
#define LEDPORT PORTB //port(this is a register)
#define LEDDDR DDRB   //register needed to set pin to input or output
#define LEDDDRPIN DDB5  //register digit(aka bit) needed to set pin to input or output

#define BTNIN DDRD
#define BTNPORT PORTD
#define BTNDDR PIND //for setting as input
#define BTNPIN PD2
uint8_t btnState=0; //variable for storing button state

int main(void){
	//enable pin as output 
	LEDDDR|= (1<<LEDDDRPIN);//if instead wanted to set to input do: LEDDDR&= ~(1<<LEDDDRPIN);
	BTNIN &= ~(1<<BTNPIN);//button(PD2) as input
        BTNPORT |= (1<<BTNPIN);//enable internal pull-up

	while (1) {
		btnState=!(BTNDDR & (1<<BTNPIN));  //returns 1 when pressed
		if(btnState){
			//led on
			LEDPORT|= (1<<LEDOUT); //write 1 for on(because  not negation(~) we do |)
		}else{
			//led off 
			LEDPORT &= ~(1<<LEDOUT);//write 0 for off (because negation(~) we do &)(doing LEDPORT&=(0<<LEDOUT); does the same thing
		}

	}

 return(0);
}

