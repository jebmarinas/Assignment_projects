
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>






volatile uint8_t of;



void out()
{
	TCCR0A = 0;					//this will activate normal mode
	TCCR0B  = 0X05;				//this will set the precaler to 1024
	TCNT0 = 0X00;				//this has the value of the counter at 0
	TIMSK0 = (1<<TOIE0);		//this will enable the itmer interrupt
	sei();						//this will enable global interrupt
}



ISR(TIMER0_OVF_vect)
{
	of++;					//this will increment the over flow
}



void press()
{
	DDRB = (0<<3);				// this turns off port b
	of = 0;						// sets the overflow back to 0
	out();					
	DDRB |= (1<<2);				//this will turn on prtb2
	while (of < 81);
	DDRB = (0<<2);				//this will turn off portb
	of = 0; // sets overflow to 0
}



int main(void)
{
	DDRC &=~ (1<<3);			//sets port c input
	PORTC |= (1<<3);			//this will activate pull up
	out();						
	DDRB = (1<<3);				//turns on port b
	while (1)
	{
		
		if (!(PINC & (1<<3)))
		{
			press();			
		}
		
		if ((of >= 15) & (of < 38))
		{
			DDRB = (0<<3);		//turns off port b
		}
		
		if ((of > 38))
		{
			DDRB = (1<<3);		//turn on portb.3
			of = 0;		//will set over flow back to 0
		}
	}
	return 1;
}


