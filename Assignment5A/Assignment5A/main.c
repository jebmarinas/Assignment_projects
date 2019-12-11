/*
 * Assignment5A.c
 *
 * Created: 12/11/2019 2:43:15 PM
 * Author : marinj4
 */ 



 #ifndef F_CPU
 #define F_CPU 16000000UL
 #endif

 #ifndef BAUD
 #define BAUD 9600
 #endif

 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>
 #include <stdbool.h>
 #include <stdio.h>
 #include <string.h>
 #include <util/setbaud.h>

 // Set up UART for printf();

 #include "inc\STDIO_UART.h"

 // Include nRF24L01+ library
 #include "inc\nrf24l01.h"
 #include "inc\nrf24l01-mnemonics.h"
 #include "inc\spi.h"
 void print_config(void);

 // Used in IRQ ISR
 volatile bool message_received = false;
 volatile bool status = false;
 volatile float adc_temp = 0;

 //uint8_t rx_address[5] = {0x72, 0x72, 0x72, 0x72, 0x72}; // Read address
 //uint8_t tx_address[5] = {0x44, 0x44, 0x44, 0x44, 0x44}; // Write address

 void USART_init(void) {		// INITIALIZE UART
	 UBRR0H = UBRRH_VALUE;	// SET HIGH BAUD REGISTER
	 UBRR0L = UBRRL_VALUE;	// SET LOW BAUD REGISTER
	 UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); //8-BIT DATA
	 UCSR0B = _BV(RXEN0) | _BV(TXEN0); //ENABLE RX AND TX
 }

 void USART_tx_string(char*data) {  // SEND STRING TO UART
	 while((*data!='\0')) {
		 while(!(UCSR0A & (1 << UDRE0)));  // WAIT FOR BUFFER TO CLEAR
		 UDR0 = *data;  // LOAD DATA INTO BUFFER REGISTER
		 data++;		// POINT TO NEXT CHARACTER IN STRING
	 }
 }

 void adc_init(void) {
	 ADMUX = (1<<REFS0) |  // AVcc set 5V
	 (0<<REFS1) |
	 (1<<MUX2)  |  // Channel 4 ADC4
	 (0<<ADLAR);   // Right justify
	 ADCSRA = (1<<ADEN) |  // Enable ADC
	 (1<<ADPS2) | // Clock/128
	 (1<<ADPS1) |
	 (1<<ADPS0);
 }

 void read_adc(void) {
	 unsigned char i = 4; // Variable to loop
	 adc_temp = 0;  // Set starting value to zero
	 while(i--) {
		 ADCSRA |= (1<<ADSC); // Start conversion
		 while(ADCSRA & (1<<ADSC)); // Wait for conversion
		 adc_temp += ADC;  // Read value from ADC
		 _delay_ms(50);
	 }
	 adc_temp = adc_temp / 4; // Average of 4 samples
 }

 void print_config(void){
	 uint8_t data;
	 printf("Startup successful\n\n nRF24L01+ configured as:\n");
	 printf("-------------------------------------------\n");
	 nrf24_read(CONFIG,&data,1);
	 printf("CONFIG 0x%02X\n",data);
	 nrf24_read(EN_AA,&data,1);
	 printf("EN_AA 0x%02X\n",data);
	 nrf24_read(EN_RXADDR,&data,1);
	 printf("EN_RXADDR 0x%02X\n",data);
	 nrf24_read(SETUP_RETR,&data,1);
	 printf("SETUP_RETR 0x%02X\n",data);
	 nrf24_read(RF_CH,&data,1);
	 printf("RF_CH 0x%02X\n",data);
	 nrf24_read(RF_SETUP,&data,1);
	 printf("RF_SETUP 0x%02X\n",data);
	 nrf24_read(STATUS,&data,1);
	 printf("STATUS 0x%02X\n",data);
	 nrf24_read(FEATURE,&data,1);
	 printf("FEATURE 0x%02X\n",data);
	 printf("-------------------------------------------\n\n");
 }


 int main(void){
	 
	 char tx_message[32]; // Define string array
	 char *tx_ptr = tx_message;
	 
	 // Initialize UART
	 USART_init();
	 
	 // Initialize nRF24L01+ and print configuration info
	 nrf24_init();
	 print_config();
	 adc_init();
	 
	 // Start listening to incoming messages
	 printf("start listening\n");
	 nrf24_start_listening();
	 printf("Done listening\n");
	 
	 while (1){

		 read_adc();
		 tx_ptr = tx_message;
		 snprintf(tx_message, sizeof(tx_message), "%f\r\n", adc_temp);  // CONVERT FLOAT TO STRING
		 nrf24_send_message(tx_ptr);
		 _delay_ms(100);

		 if (message_received){
			 printf("inside if condition\n");
			 // Message received, print it
			 message_received = false;
			 printf("Message Received: %s\n",nrf24_read_message());
			 // Send message as response
			 _delay_ms(500);
			 status = nrf24_send_message(tx_message);
			 if (status == true)
			 printf("Message Transmitted\n");
		 }
	 }
 }

 // Interrupt on IRQ pin
 ISR(INT0_vect) {
	 message_received = true;
 }