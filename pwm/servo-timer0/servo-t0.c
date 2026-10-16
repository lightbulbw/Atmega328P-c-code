 
//servo controlled with timercounter0 example 
#include <avr/io.h>//this library is needed to use IO pins(input/output pins)

//for uno and nano use 16MHz
#define F_CPU 16000000UL  // 16 MHz

//this library contains _delay_ms(x);
#include <util/delay.h>//this line needs to be after #define F_CPU because delay needs F_CPU

//using arduino pin6, aka PD6, aka OC0A (on concordia board its port p2)
#define SERVOOUT PORTD6 //pin (bit)
#define SERVOPORT PORTD //port(this is a register)
#define SERVODDR DDRD   //register needed to set pin to input or output
#define SERVODDRPIN DDD6  //register digit(aka bit) needed to set pin to input or output

uint8_t cnt=7;//this variable has value of intensity. 7 not 0 initially because 0 is no pulse and servo wants to always get a signal

//initialize pwm(pulse width modulation)
void init_pwm(void) {
	TCNT0=0;// set timercounter 2 register to zero
	OCR0A=0;//OCR2A(Output Compare Register A) constantly compares itself to TCNT0 to know when to generate output at OC0A pin
	
	//TCCR0A(Timer/Counter Control Register A) contains bits(COM0A1,COM0A0,WGM01,WGM00, and others)
	// COM0A1  COM0A0  
	//   0      0 Normal port operation, OC0A disconnected.
	//   0      1 Toggle OC0A on Compare Match
	//   1      0 Clear OC0A on Compare Match(Set output to low level)
	//   1      1 Set OC0A on Compare Match(Set output to high level)
	
	// Fast PWM, Phase Correct, 8-bit (always count up, wrap counter at top)
	TCCR0A=(1<<COM0A1)|(0<<COM0A0)|(1<<WGM01)|(1<<WGM00); 
	//we need TCCR2B(Timer/Counter Control Register B) to set the prescaler
        //servo needs 20ms duty cycle so to get from 16E6Hz(clock speed) to 1/20E-3=50Hz we need 16E6/50/255=1254 so closest prescaler is 1024
        //(we devide by 255 because that is the max count of a 8 bit register and we want to convert this to max period being 20ms.)
	TCCR0B=(1<<CS02)|(0<<CS01)|(1<<CS00); // 1/1024 of clock speed
        //CS02 CS01 CS00 
        //0      0    0   No clock source (Timer/Counter stopped).
        //0      0    1   clk T2S /(No prescaling)
        //0      1    0   clk T2S /8 (From prescaler)
        //0      1    1   clk T2S /32 (From prescaler)
        //1      0    0   clk T2S /256 (From prescaler)
        //1      0    1   clk T2S /1024 (From prescaler)
        //1      1    0   External clock source on T0 pin. Clock on falling edge.
        //1      1    1   External clock source on T0 pin. Clock on rising edge.

}

int main(void){
	//enable pin as output 
	SERVODDR|= (1<<SERVODDRPIN);
	//initialize pwm
	init_pwm();
	 
	while (1) {
//the exact value of cnt depend on the specific servo

                 _delay_ms(1000);
                cnt=7;//0degrees relative to reference point of servo
		OCR0A=cnt; //set register to wanted pulse length
		_delay_ms(1000);
        
                cnt=20;//90degrees relative to reference point of servo
		OCR0A=cnt;
                _delay_ms(1000);
                
                 cnt=38;//180degrees relative to reference point of servo
		OCR0A=cnt;
                _delay_ms(1000);

                cnt=20;//90degrees relative to reference point of servo
		OCR0A=cnt;
                _delay_ms(1000);
	}

 return(0);
}
