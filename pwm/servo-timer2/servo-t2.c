  
//servo controlled with timercounter2 example
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

uint8_t cnt=7;//this variable has value of intensity. 7 not 0 initially because 0 is no pulse and servo want to always get a signal

//initialize pwm(pulse width modulation)
void init_pwm(void) {
        //pin PB3 is connected to OC2A pin(which is the output pin of OCR2A)
	//TCNT2 is timercounter 2 register. 
	TCNT2=0;// set counter to zero
	// led intensity(OCR2A)=0 -> led =off
	// led intensity(OCR2A)=0xFF -> led =fully on
	OCR2A=0;//OCR2A(Output Compare Register A) constantly compares itself to TCNT2 to know when to generate output at OC2A pin
	
	//TCCR2A(Timer/Counter Control Register A) contains bits(COM2A1,COM2A0,WGM21,WGM20, and others)
	// COM2A1  COM2A0  
	//   0      0 Normal port operation, OC2A disconnected.
	//   0      1 Toggle OC2A on Compare Match
	//   1      0 Clear OC2A on Compare Match(Set output to low level)
	//   1      1 Set OC2A on Compare Match(Set output to high level)
	
	// Fast PWM, Phase Correct, 8-bit (always count up, wrap counter at top)
	TCCR2A=(1<<COM2A1)|(0<<COM2A0)|(1<<WGM21)|(1<<WGM20); 
	//we need TCCR2B(Timer/Counter Control Register B) to set the prescaler
	//servo needs a very slow pulse
        //servo needs 20ms duty cycle so to get from 16E6Hz(clock speed) to 1/20E-3=50Hz we need 16E6/50/255=1254 so closest prescaler is 1024
        //(we devide by 8bit(255) because 255 is the max period of a 8 bit register and we want to convert this to max period being 20ms.)
	TCCR2B=(1<<CS22)|(1<<CS21)|(1<<CS20); // 1/1024 of clock speed
        //CS22 CS21 CS20 
        //0      0    0   No clock source (Timer/Counter stopped).
        //0      0    1   clk T2S /(No prescaling)
        //0      1    0   clk T2S /8 (From prescaler)
        //0      1    1   clk T2S /32 (From prescaler)
        //1      0    0   clk T2S /64 (From prescaler)
        //1      0    1   clk T2S /128 (From prescaler)
        //1      1    0   clk T2S /256 (From prescaler)
        //1      1    1   clk T2S /1024 (From prescaler)

}

int main(void){
	//enable pin as output 
	LEDDDR|= (1<<LEDDDRPIN);
	//initialize pwm
	init_pwm();
	 
	while (1) {
//cnt is 8 bit so can take any value between 0 and 254. 20ms=254 so to get 1ms we need 254/20=13, for 90deg:1.5ms*254/20=19, for 180deg:2ms*254/20=26
//however,I think the exact numbers depend on the specific servo.
//using the one from school(HS-422) min is 7(0degrees) and max is 38(180degrees) and 20 is 90degrees and if out of range it gitters.
//however using MG90s(black) where 3 is lowest value(overshots to like -30 degrees) 7=0degrees, 25=90degrees, 40 does 180degrees and at 41 does 360 degrees
//(this servo is special and can do not just 180 but full 360) and 42 and above or below 3 it just does not turn(so stays at previous pos)
//while using SG90(blue): below 5 it refuses to turn,23=90deg,38=180deg,above 40 it does not move

//(also interesting SG90(blue) needs exactly 5v which is the standard servo voltage. MG90s(black) is able to run with both 5v and 3.3v).

                 _delay_ms(1000);
                cnt=7;//0 degrees relative to reference point of servo
		OCR2A=cnt; //set register to wanted pulse length
		_delay_ms(1000);
        
                cnt=25;//90degrees relative to reference point of servo
		OCR2A=cnt;
                _delay_ms(1000);
                
                 cnt=40;//180degrees relative to reference point of servo
		OCR2A=cnt;
                _delay_ms(1000);
	}

 return(0);
}
