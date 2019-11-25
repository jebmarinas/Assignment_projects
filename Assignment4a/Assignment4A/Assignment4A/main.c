
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

volatile float adc_temp;


void adc_init(void)
{
	ADMUX = (0<<REFS1)| // reference selection bit
	(1<<REFS0)| //AVCC EXTERNAL CAP AT REF
	(1<<ADLAR)| //ADC LEFT ADJUST RESULT
	(0<<MUX2)| //ANALOG CHANNEL SELECTION BITS
	(0<<MUX1)| //ADC
	(0<<MUX0);
	ADCSRA = (1<<ADEN)| // ADC ENABLE
	(0<<ADSC)| // ADC START CONVERSION
	(0<<ADATE)| // ADC AUTO TRIGGER ENABLE
	(0<<ADIF)| // ADC INTERRUPT ENABLE
	(0<<ADIE)|// ADC PRESCALER SELECT BIT
	(1<<ADPS2)|
	(1<<ADPS1)|
	(1<<ADPS0); // SELECT CHANNEL
}
void read_adc(void)
{
	unsigned char i = 4;
	adc_temp = 0;
	while(i--)
	{
		ADCSRA |=(1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp += ADC;
		_delay_ms(50);
	}
	adc_temp = (adc_temp/8);
}
ISR(PCINT1_vect)
{
	DDRC ^= (1<<0);
}
int main()
{
	adc_init();
	DDRC |= (0<<3);
	PORTC |= (1<<3);
	PORTC |= (1<<0);
	DDRB |= (1<<DDB1);
	
	PCICR = (1<<PCIE1);
	PCMSK1 = (1<<PCINT11);
	sei();

	ICR1 =0xFFFF;
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1);
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	TCCR1B |= (1<<CS10);

	while(1)
	{
		read_adc();
		_delay_ms(50);
		if((adc_temp >= 62237) && (adc_temp < 65456))
		{
			OCR1A = 62237;
			_delay_ms(50);
		}
		else
		{
			OCR1A = 0x00;
		}
		return 0;
	}
}
