
//NEW 2C TASK1/A W/O BUTTON
// TCNT_ON = (((16MHZ/1024)0.435)-1) = 6796
//TCNT_OFF =(((16MHZ/1024)0.29)-1) = 4530
#define F_CPU 16000000UL
#include <avr/io.h>

int main(void)
{
	//DDRB |= (1<<2); //
	//PORTB &=~(1<<2);
	while(1)
	{
		TCNT0 =0;
		TCCR0A = 0x00;
		TCCR0B = 0x05;
		//DDRB |= (1<<3);
		int count = 0;
		
		DDRB = (1<<3);
		while(count != 15)// 40% DC ON 4530/255 = 18
		{
			while((TIFR0 & 0x01) ==0);
			TIFR0 |= (1<<TOV0);
			
			count++; //increments count
		}
		//TCNT0 =0;// sets the timer to 0
		//PORTB^=(1<<2); TOGGLES PORTB
		// SETS THE COUNTER BACK TO 0
		TCNT0 =0;
		TCCR0A = 0x00;
		TCCR0B = 0x05;
		DDRB = (0<<3);
		int count2 = 0;
		while(count2 != 23)// 60 % DC OFF 6796/255= 27
		{
			//DDRB =(0<<3);
			while((TIFR0 & 0x01) ==0);
			TIFR0 |= (1<<TOV0);
			count2++;// increments count
		}
		DDRB = (1<<3);
		//TCNT0 =0;// resets timer to 0
	}
}