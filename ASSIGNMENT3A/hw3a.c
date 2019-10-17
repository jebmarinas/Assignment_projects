
#define BAUD 9600
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_send(char info);// this will output the char function
void USART_putstring(char* str);// this will output the string fucntion
void USART_init(void);// void function


char str_Array[] = "hello world Jeb Marinas Assignment 3a";// variable that will output into terminal
char count[100];// array will have up to 100 characters inside the array.
volatile float num = 9.95;//this is my grade after

int main(void)
{
	TCCR1B = 0x05; // 1024 prescaler
	TIMSK1 = (1<<TOIE1); //  enable will enable the overflow Interrupt
	TCNT1 = 49911;//
	USART_init();// calls function
	sei();// enable interrupt
	
	while(1)
	{
		// main loop
	}
}

ISR (TIMER1_OVF_vect)
{
	USART_putstring(str_Array); // prints string
	USART_putstring("\n"); // create a new line.
	USART_send('1');//sends “1"into the stream
	USART_putstring("\n"); // will create a new line.
	snprintf(count, sizeof(count), "%f\r\n", num);//he floating point characters are stored in outs
	USART_putstring(count);//grabs count
	TCNT1 = 49911; // reset
}
void USART_init( void )
{
	UBRR0H = 0; //USART Baud Rate Register high
	UBRR0L = F_CPU/16/BAUD - 1;// USART Baud Rate Register low: BAUD prescaler
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8 bit data
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   // Enable RX and TX
}
void USART_send(char info) // sends data/ integer
{
	while (!(UCSR0A & (1 << UDRE0)));//USART Control and Status Register 
	UDR0 = info;
}
void USART_putstring(char *str)// print string into the terminal
{
	// this while statement will increment my string one by one until its not '0'
	while ((*str != '\0')){
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *str;
		str++;
	}
}
