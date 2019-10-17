
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t of;

 void out();
 void press();

int main(void)
{
	DDRC &=~ (1<<3);			//sets portc as an input
	PORTC |= (1<<3);			//this will activate pull up resistor
	out();			
	DDRB = (1<<3);				//turns on portb
	while (1)
	{
		
		if (!(PINC & (1<<3)))
		{
			press();
		}
		
		if((of >= 30) & (of < 76))

		{
			DDRB = (0<<3);		//turns of port b
		}
		
		if ((of > 76))
		{
			DDRB = (1<<3);		//turns on port.3
			of = 0;  // this will set overflow back to 0
		}
	}
	return 1;
}
void out(){
	TCCR0A = (1<<WGM01);		//this will activate ctc mode
	TCCR0B  = 5;				//this will set prescaler to 1024
	TCNT0 = 0X00;				//this will set the value of the counter to 0
	OCR0A = 0X80;				//this will have the value of the register to 128
	TIMSK0 = (1<<OCIE0A);		//this will start the counterc compare interrupt
	sei();						//this will start global interrupt
}
void press()
{
	DDRB = (0<<3);				//turns port b off
	of = 0;		// this will set overflow back to 0
	out();
	DDRB |= (1<<2);				//sets port b on
	while (of < 162 );
	DDRB = (0<<2);				//sets port b off
	of = 0;// sets over flow back to 0
}