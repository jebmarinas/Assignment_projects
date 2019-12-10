#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void adc_init(void);
void read_adc(void);

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
    volatile int control = 0; //this will control the motor
    volatile unsigned int speed; // this will control how was the motor go

int main(void)
{
      DDRB = 0x0F;//will be used as an output
      PORTB = 0x00// the voltage in it will be 0
      TCCR1B = (1 << WGM13)|(1 << WGM12)|(1 << CS11);// THE PRESSCALAR WILL BE AT 8
      ICR1 = 62258;

      adc_init();
      while(1)
      {
            read_adc();
            _delay_ms(30);
      }
      while(1)
      {
            PORTD = 0b01100110;
            _delay_ms(50);
            PORTD = 0b11001100;
            _delay_ms(50);
            PORTD = 0b10011001;
            _delay_ms(50);
            PORTD = 0b00110011;
            _delay_ms(50);

            if ((adc >= 981) && (adc < 1024 ))	//when adc value is at its max
            {
                  control = 1;
                _delay_ms(20);
            }
            else if ((adc < 982) && (adc >= 758)) //will decrease pwm
            {
                    control = 0;
                    speed = 0x1869;
                  _delay_ms(50);
            }
            else if ((adc < 770) && (adc >= 67)) //will decreasing pwm
            {
                  control = 0;
                  speed = 0x124F;
                  _delay_ms(100);
            }
            else
            {
                  PORTD = 0b00000000;//if resistor value is less than 5%

                  _delay_ms(100);
            }

        }

}
