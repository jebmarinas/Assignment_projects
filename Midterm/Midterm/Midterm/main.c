
#define F_CPU 16000000UL
#define BAUD 9600
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include <stdio.h>

volatile uint8_t of;
volatile float adc_temp;

char direct1[] = "AT";
char direct2[] = "AT+CWMODE=1"; //this will enable the wifi
char direct3[] = "AT+CWLAP"; // list wireless wifi AP in range
char direct4[] = "AT+CWJAP=\"WIFI-NAME\",\"PASSWORD\""; // this will join the network
char direct5[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80";
char direct6[] = "AT+CIPSEND=51 \\";
char direct7[] = "AT+CIPCLOSE";

void usart_init(void)
{
	UBRR0H = UBRRH_VALUE; // loads the ubrrh value into high
	UBRR0L = UBRRL_VALUE;// loads the ubrr0l value into low
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8-bit data
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);// enable RX and TX
}
void set()
{
	TCCR0A = 0; // normal mode is initiated
	TCCR0B = 0x05;// the prescaler is set to 1024
	TCNT1 = 0x00;// initiates the counter to 0
	TIMSK0 = (1<<TOIE0); // enable interrupt
	sei(); // enable global interrupt
}
void usart_tx_string(char * info)
{
	while((*info != '\0'))
	{
		while( !(UCSR0A & (1 << UDRE0)));
		UDR0 = *info;
		info++;
	}
}
ISR(TIMER0_OVF_vect)
{
	of++; // this will increment the overflow
}
void adc_init(void)
{
	ADMUX = (0<<REFS1)| // reference selection bit
	(1<<REFS0)| //AVCC EXTERNAL CAP AT REF
	(0<<ADLAR)| //ADC LEFT ADJUST RESULT
	(1<<MUX2)| //ANALOG CHANNEL SELECTION BITS
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
	adc_temp = (adc_temp/4);
}
int main(void)
{
	usart_init();
	set();
	adc_init();
	
	char c[256];

	usart_tx_string(direct1);
	_delay_ms(6000);
	usart_tx_string(direct2);
	_delay_ms(6000);
	usart_tx_string(direct3);
	_delay_ms(6000);
	usart_tx_string(direct4);
	_delay_ms(6000);
	usart_tx_string(direct5);
	_delay_ms(6000);
	usart_tx_string(direct6);
	_delay_ms(6000);
	usart_tx_string(direct7);
	_delay_ms(6000);

	read_adc(); // this will read the adc
	snprintf(c,sizeof(c),"GET//api.thingspeak.com/update?api_key=FRW23XEXSTDAH6SN&field1=%3d\r\n", adc_temp); // this will print out the float
	usart_tx_string(c); // this will output on the terminal
	TCNT1	=0x00; // this will reset the timer

	usart_init();
	set();
	adc_init();
	
	

	while(1)
	{
		while(of < 4);
		read_adc();
		snprintf(c,sizeof(c), "%f\r\n", adc_temp);
		usart_tx_string(c);
		usart_tx_string("\n");
		of = 0;
	}
}







