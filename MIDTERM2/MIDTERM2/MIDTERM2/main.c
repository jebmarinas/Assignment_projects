
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 115200
#define FOSC 16000000
#define UBRR FOSC/8/BAUD-1

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "APDS9960_def.h"
#include "i2c_master.h"
void getValues(void);
void init_APDS9960(void);
void usart_init();
void USART_putstring(volatile unsigned char *StringPtr);

volatile unsigned char AT[] = "AT\r\n";
volatile unsigned char CWMODE[] = "AT+CWMODE=3\r\n";
volatile unsigned char CWJAP[] = "AT+CWJAP=\"blahblah\",\"blahblah\"\r\n";
volatile unsigned char CIPSTART[] = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n";
volatile unsigned char CIPSEND[] = "AT+CIPSEND=104\r\n";
volatile unsigned char CIPMUX[] = "AT+CIPMUX=0\r\n";
volatile unsigned char SEND_DATA[] = "GET /update?key=IW250NQFTF4HL1KZ&field1=";
volatile unsigned char RESET[] = "AT+RST\r\n";
volatile unsigned char LINEBREAK[] = "\r\n";
volatile unsigned char CLOSE[] = "AT+CIPCLOSE\r\n";


volatile unsigned char red[10];
volatile unsigned char green[10];
volatile unsigned char blue[10];

uint16_t val_red, val_green, val_blue;


int main(void){
	i2c_init();
	usart_init(115200);
	init_APDS9960();

	_delay_ms(10);
	USART_putstring(AT);					//this will send AT to the USART

	//connect to network
	_delay_ms(10);
	USART_putstring(RESET);		//this willreset ESP
	_delay_ms(10);
	USART_putstring(AT);
	_delay_ms(10);
	USART_putstring(CWMODE);		//where the wifi mode gets picked
	_delay_ms(10);
	USART_putstring(CWJAP);


	while(1){

		_delay_ms(10);
		USART_putstring(CIPMUX);
		_delay_ms(10);
		USART_putstring(CIPSTART);
		_delay_ms(10);
		USART_putstring(CIPSEND);
		_delay_ms(10);
		getValues();
		USART_putstring(SEND_DATA);
		USART_putstring(red);
		USART_putstring("&field2=");
		USART_putstring(green);
		USART_putstring("&field3=");
		USART_putstring(blue);
		_delay_ms(1000);

	}

	return 0;
}

void usart_init() {
	UBRR0H = ((UBRR) >> 8);
	UBRR0L = UBRR;
	UCSR0A |= (1<< U2X0); // divisor baud = 8
	UCSR0B |= (1 << TXEN0); // Enable transmission
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8 bits
}

void USART_putstring(volatile unsigned char *StringPtr)
{
	while ((*StringPtr != '\0')){
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *StringPtr;
		StringPtr++;
	}
}

void init_APDS9960(void){
	uint8_t value;

	i2c_readReg(APDS9960_WRITE, APDS9960_ID, &value,1);
	if(setup != APDS9960_ID_1) while(1);
	value = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	i2c_writeReg(APDS9960_WRITE, APDS9960_ENABLE, &value, 1);
	value = DEFAULT_ATIME;
	i2c_writeReg(APDS9960_WRITE, APDS9960_ATIME, &value, 1);
	value = DEFAULT_WTIME;
	i2c_writeReg(APDS9960_WRITE, APDS9960_WTIME, &value, 1);
	value = DEFAULT_PROX_PPULSE;
	i2c_writeReg(APDS9960_WRITE, APDS9960_PPULSE, &value, 1);
	value = DEFAULT_POFFSET_UR;
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_UR, &value, 1);
	value = DEFAULT_POFFSET_DL;
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_DL, &value, 1);
	value = DEFAULT_CONFIG1;
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG1, &value, 1);
	value = DEFAULT_PERS;
	i2c_writeReg(APDS9960_WRITE, APDS9960_PERS, &value, 1);
	value = DEFAULT_CONFIG2;
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG2, &value, 1);
	value = DEFAULT_CONFIG3;
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG3, &value, 1);
}


void getValues(void){
uint8_t high_red, low_red;
uint8_t high_green, low_green;
uint8_t high_blue, low_blue;

unsigned char i;
char test[10];



i2c_readReg(APDS9960_WRITE, APDS9960_RDATAH, &high_red, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_RDATAL, &low_red, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_GDATAH, &high_green, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_GDATAL, &low_green, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_BDATAH, &high_blue, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_BDATAL, &low_blue, 1);

val_red = (high_red << 8) | low_red;
val_green = (high_green << 8) | low_green;
val_blue = (high_blue << 8) | low_blue;


if (val_red > 255){
val_red = 255;
}
if (val_green > 255){
val_green = 255;
}
if (val_blue > 255){
val_blue = 255;
}

	itoa(val_red, test, 10);
	for(i = 0 ; i < 10 ; i++){
		red[i] = test[i];
	}
	itoa(val_green, test, 10);
	for(i = 0 ; i < 10 ; i++){
		green[i] = test[i];
	}
	itoa(val_blue, test, 10);
	for(i = 0 ; i < 10 ; i++){
		blue[i] = test[i];
	}


}
