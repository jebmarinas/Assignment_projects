#define F_CPU 16000000UL
#include <avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>

int main()
{
	DDRB = (1<<3);
	PORTD = (1<<2);
	EICRA = 0x2;
	EIMSK = (1<<INT0);
	sei();
	while (1)
	{
		PORTB ^=(1<<3);
		_delay_ms(250);
		PORTB ^=(1<<3);
		_delay_ms(375);
	}
}
ISR(INT0_vect)
{
	PORTB &= ~(1<<3)
	DDRB  &= ~(1<<3)
	DDRB |= (1<<2)
	PORTB ^=(1<<3);
	_delay_ms(1333);
	DDRB &= ~(1<<2)
	PORTB &= ~(1<<2)
	DDRB |= (1<<3)
	
	
	
}