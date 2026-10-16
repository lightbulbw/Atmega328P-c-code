 
//serial port example 
#define F_CPU 16000000UL//16MHz
#include <avr/io.h>//this library is needed to use IO pins(input/output pins)
#include <avr/interrupt.h>//needed to handle interrupts
#include <stdlib.h>//standard library/general utilities
#include <string.h>//to send strings we need this library
#include <util/delay.h>//this library contains _delay_ms(x);

//define variables to set speed of serial comunication
#define BAUD 9600UL
#define UBRR ((F_CPU)/((BAUD)*(16UL))-1)//UBRR(UART Baude Rate Register)

//to set flags
volatile struct {
  uint8_t TX_finished:1;
  uint8_t sample:1;
} flags;

static const char CRLF[3]={13, 10};//Carriage Return (ASCII 13, \r) Line Feed (ASCII 10, \n)

static volatile char *msg, TX_buffer1[10];//needs to be volatile because gets modified by interrupt

//=====================
//only transmission
void uart_tx_init(void) {
// Set the UART speed as defined by UBRR
  UBRR0H = (uint8_t)((UBRR)>>8); //load upper 8 bits
  UBRR0L = (uint8_t)UBRR;//load lower 8bits

  UCSR0B|=(1<<TXCIE0)|(1<<TXEN0); //(1<<UDRIE0) Enable TX and TX IRQ.
  UCSR0C=(3<<UCSZ00); // Asynchronous UART, 8-N-1
}

//both transmitting and reciving
void uart_init(void) { //TX and RX init with IRQ(interrupt request)
// Set the UART speed as defined by UBRR
  UBRR0H = (uint8_t)((UBRR)>>8); //load upper 8 bits
  UBRR0L = (uint8_t)UBRR;//load lower 8bits

  UCSR0B|=(1<<TXCIE0)|(1<<TXEN0); // Enable TX and TX IRQ.
  UCSR0B|=(1<<RXCIE0)|(1<<RXEN0); // Enable RX and RX IRQ 
  UCSR0C=(3<<UCSZ00); // Asynchronous UART, 8-N-1
}


//======================
// UART TX ISR(interrupt service routine)
// it loads UDR as long as the "msg" contains non-zero characters
// once it finds a zero character, it sets TX_finished flag, so a new process can start transmission
//look up interrupt vector names https://ece-classes.usc.edu/ee459/library/documents/avr_intr_vectors/
ISR (USART_TX_vect) {
  msg++;
  if (*msg) UDR0=*msg;
  else flags.TX_finished=1;
}

//=========================
void send_int(int16_t data, uint8_t base, uint8_t crlf){//ex:15, 10, 1 here data=15, base =10, 1 to send new line
	while (!flags.TX_finished); //waiting for other transmission to complete
	flags.TX_finished=0;
	itoa(data, (char*) &TX_buffer1[0], base);
	if (crlf) strcat((char*)TX_buffer1, CRLF);
	msg=TX_buffer1;
	UDR0=*msg;
}

void send_string (char *str) {
	while (!flags.TX_finished); //waiting for other transmission to complete
	msg=str;
	UDR0=*msg; 
	flags.TX_finished=0;
}

//============================
int main(void){
	uart_tx_init();
	flags.TX_finished=1;

	sei(); // enable interrupt
	flags.sample=1;
	
	
	while (1) { //infinie loop		
		send_string("hi\r\n");
		send_int(15,10,1);
		_delay_ms(1000);
		send_string("shark\r\n");
		send_string("yellow\r\n");
		_delay_ms(1000);
	}


 return(0);
}
