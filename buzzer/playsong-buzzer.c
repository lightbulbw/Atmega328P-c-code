/*************
*active buzzer attacehd to pin 3 and GND
*using timer interrupt to set the beat.

inspired from the arduino code from: https://www.build-electronic-circuits.com/arduino-buzzer/
***********/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define BUZZERDDR   DDRD
#define BUZZERPORT  PORTD
#define BUZZERPIN   PD3

//note frequencies in Hz
#define NOTE_C4   262
#define NOTE_D4   294
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_G4   392
#define NOTE_A4   440
#define NOTE_AS4  466
#define NOTE_C5   523

//for happy birthday
/*static uint16_t melody[] = {
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
    NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
    NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

static uint8_t noteDurations[] = {
    4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 2
};*/
//jingle bells
/*static uint16_t melody[] = {
  //Jingle bells, jingle bells, jingle all the way 
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
    //Oh what fun it is to ride in a one-horse open sleigh 
    NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
    NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4
};

static uint8_t noteDurations[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8, 2,

    8, 8, 8, 8,
    8, 8, 8, 8, 4,
    8, 8, 8, 8, 2, 2
};*/
//twinkle twinkle little star
//twinkle twinkle little star
static uint16_t melody[] = {
    // Twinkle, twinkle, little star,
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
    // How I wonder what you are.
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
    // Up above the world so high,
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
    // Like a diamond in the sky.
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
    // Twinkle, twinkle, little star,
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
    // How I wonder what you are.
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

static uint8_t noteDurations[] = {
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 4, 2
};

#define TICKRATE 10000UL  //timer interrupt rate: 10 kHz 
#define COUNT (sizeof(melody) / sizeof(melody[0]))

volatile uint8_t note = 0;
volatile uint8_t finished = 0;

volatile uint16_t elapsed = 0;
volatile uint16_t soundticks;
volatile uint16_t totalticks;
volatile uint16_t halfperiod;
volatile uint16_t phase = 0;

static void loadnote(void){
    uint16_t quarternotems = 550;  //this adjusts tempo of song
    uint16_t ms = quarternotems * 4 / noteDurations[note];

    soundticks = ms * 10;
    totalticks = (uint16_t)((uint32_t)soundticks * 130 / 100);//(uint16_t)((uint32_t) to fix truncation issue
    halfperiod = TICKRATE / (2UL * melody[note]);

    elapsed = 0;
    phase = 0;
}

ISR(TIMER0_COMPA_vect)
{
    if(finished){
        return;
    }
    elapsed++;

    if(elapsed <= soundticks){
        phase++;

        if(phase >= halfperiod){
            phase = 0;
	    //toggle the buzzer pin
            BUZZERPORT ^= (1 << BUZZERPIN);//^ is XOR
        }
    }else{
	//turn buzzer off
        BUZZERPORT &= ~(1 << BUZZERPIN);
    }
}

void timer0init(void) {
	TCNT0=0;// set timercounter 0 register to zero
	//16 MHz / 8 / 200 = 10 kHz => want 200 ticks per interrupt for prescaler 8
	OCR0A=199;//OCR0A(Output Compare Register A) constantly compares itself to TCNT0 to know when to generate output at OC0A pin
	
	TCCR0A=(0<<COM0A1)|(0<<COM0A0)|(1<<WGM01)|(0<<WGM00); //CTC mode
	TIMSK0 = (1 << OCIE0A);  // Enable compare-match interrupt
	TCCR0B=(0<<CS02)|(1<<CS01)|(0<<CS00); // 1/8 of clock speed
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

//============
int main(void){

	BUZZERDDR|= (1<<BUZZERPIN);//initialize buzzer as output

    	timer0init();

    	//play song
	loadnote();
	while (!finished) {
    		uint16_t ticks;

    		cli();
    		ticks = elapsed;
    		sei();

    		if (ticks >= totalticks) {
        		cli();

        		note++;

        		if(note >= COUNT){
            			finished = 1;
            			TIMSK0 = 0;
            			TCCR0B = 0;
            			BUZZERPORT &= ~(1 << BUZZERPIN);
        		}else{
            			loadnote();
        		}

        		sei();
    		}
	}
    	
	while(1) {
        // Keep the MCU here so it doesn't reset or crash
    	}
    	return(0);
}
