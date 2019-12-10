#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void adc_init(void);
void read_adc(void);
volatile float adc
int check = 0;

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
    (1<<ADATE)| // ADC AUTO TRIGGER ENABLE
    (0<<ADIF)| // ADC INTERRUPT ENABLE
    (0<<ADIE)|// ADC PRESCALER SELECT BIT
    (1<<ADPS2)|
    (1<<ADPS1)|
    (0<<ADPS0); // SELECT CHANNEL
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
int main(void)
{
    adc_init(); //Initializing ADC

	  PORTC |= 0b00000001;
	  DDRD = 0xFF; DDRB = 0xFF;//outputs

	//Configuration of Timer1
	  TCCR1A |= (1 << COM1A1) |(1 << COM1B1) |(1 << WGM11);
	  TCCR1B |= (1 << WGM13) |(1 << WGM12) |(1 << CS11) |(1 << CS10);
    ICR1 = 62258;

    while(1)
    {
        read_adc();
        _delay_ms(50);

        while(1)
        {
              ADCSRA |= (1<< ADSC);
              while((ADCSRA & (1 << ADIF)) == 0);
              check = ADCH;

              if(check == 0)
              {
                  0CR1A = 0;
                  _delay_ms(200);
              }
              if(check == 255)
              {
                    0CR1A = 500;
                    _delay_ms(200);
              }
        }


    }
  }
