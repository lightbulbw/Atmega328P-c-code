  
//dimming led pwm example
#include <avr/io.h>//this library is needed to use IO pins(input/output pins)

//_delay_ms(x) fct uses this to know how many cycles a second is
//_delay_ms(x) does not use prescaler
//for uno and nano use 16MHz
#define F_CPU 16000000UL  // 16 MHz

//this library contains _delay_ms(x);
#include <util/delay.h>//this line needs to be after #define F_CPU because delay needs F_CPU

//led on nano or uno board goes to ground(so 1 is on)
//using arduino pin11, aka PB3, aka OC2A
#define LEDOUT PORTB3 //pin (bit)
#define LEDPORT PORTB //port(this is a register)
#define LEDDDR DDRB   //register needed to set pin to input or output
#define LEDDDRPIN DDB3  //register digit(aka bit) needed to set pin to input or output

uint8_t cnt=0;//this variable has value of intensity. initially led off, cnt stands for count

//initialize pwm(pulse width modulation)
void init_pwm(void) {
        //pin PB3 is connected to OC2A pin(which is the output pin of OCR2A)
	//TCNT2 is timercounter2 register. 
	TCNT2=0;// set counter to zero
	// led intensity(OCR2A)=0 -> led =off
	// led intensity(OCR2A)=0xFF -> led =fully on
	OCR2A=0;//OCR2A(Output Compare Register A) constantly compares itself to TCNT2 to know when to generate output at OC2A pin
	
	//TCCR2A(Timer/Counter Control Register A) contains bits(COM2A1,COM2A0,WGM21,WGM20, and others)
	// COM2A1  COM2A0  
	//   0      0 Normal port operation, OC0A disconnected.
	//   0      1 Toggle OC2A on Compare Match
	//   1      0 Clear OC2A on Compare Match(Set output to low level)
	//   1      1 Set OC2A on Compare Match(Set output to high level)
	
	// Fast PWM, Phase Correct, 8-bit (always count up, wrap counter at top)
	TCCR2A=(1<<COM2A1)|(0<<COM2A0)|(1<<WGM21)|(1<<WGM20); 
	//we need TCCR2B(Timer/Counter Control Register B) to set the prescaler
	TCCR2B=(0<<CS21)|(1<<CS20); // here we set prescaler to no prescaler

}

int main(void){
	//enable pin as output 
	LEDDDR|= (1<<LEDDDRPIN);
	//initialize pwm
	init_pwm();
	 
	while (1) {
		cnt+=20; // count up and wrap at 254(wrap happens automatically)
		OCR2A=cnt; //set register to wanted intensity, led intensity 0 to 254, 254 = full on, 0 = off
		_delay_ms(500);
	}

 return(0);
}
