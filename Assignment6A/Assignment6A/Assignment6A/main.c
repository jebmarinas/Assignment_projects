/*
 * Assignment6A.c
 *
 * Created: 12/11/2019 3:40:20 PM
 * Author : marinj4
 */ 



 #define F_CPU 16000000UL
 #include <avr/io.h>
 #include <util/delay.h>
 #include <inttypes.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include "MPU6050_res_define.h"
 #include "I2C_Master_H_file.h"
 #include "USART_RS232_H_file.h"

 float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;

 void MPU6050_Init()
 {
	 _delay_ms(150);// this will power up the time
	 I2C_Start_Wait(0xD0);//this will write the address
	 I2C_Write(SMPLRT_DIV);// this will write into the register	
	 I2C_Write(0x07);									
	 I2C_Stop();

	 I2C_Start_Wait(0xD0);
	 I2C_Write(PWR_MGMT_1);//this will manage the power register
	 I2C_Write(0x01);			
	 I2C_Stop();

	 I2C_Start_Wait(0xD0);
	 I2C_Write(CONFIG);	// will write the configuration
	 I2C_Write(0x00);
	 I2C_Stop();

	 I2C_Start_Wait(0xD0);
	 I2C_Write(GYRO_CONFIG);//will write the gyro configuration
	 I2C_Write(0x18);
	 I2C_Stop();

	 I2C_Start_Wait(0xD0);
	 I2C_Write(INT_ENABLE);//will write onto the interrupt
	 I2C_Write(0x01);
	 I2C_Stop();
 }

 void MPU_Start_Loc()
 {
	 I2C_Start_Wait(0xD0);								
	 I2C_Write(ACCEL_XOUT_H);							
	 I2C_Repeated_Start(0xD1);							
 }

 void Read_RawValue()
 {
	 MPU_Start_Loc();									
	 Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	 Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	 Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	 Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	 Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	 Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	 I2C_Stop();
 }

 int main()
 {
	 char buffer[20];
	 char float_val[10];
	 float X_a,Y_a,Z_a,t;
	 float X_g=0,Y_g=0,Z_g=0;
	 I2C_Init();
	 MPU6050_Init();
	 USART_Init(9600);
	 
	 while(1)
	 {
		 Read_RawValue();

		 X_a = Acc_x/16384.0;
		 Y_a = Acc_y/16384.0;
		 Z_a = Acc_z/16384.0;
		 
		 X_g = Gyro_x/16.4;
		 Y_g = Gyro_y/16.4;
		 Z_g = Gyro_z/16.4;

		 dtostrf( X_a, 3, 2, float_val );			
		 sprintf(buffer," Ax = %s g\t",float_val);			
		 USART_SendString(buffer);						

		 dtostrf( Y_a, 3, 2, float_val );					
		 sprintf(buffer," Ay = %s g\t",float_val);			
		 USART_SendString(buffer);						
		 
		 dtostrf( Z_a, 3, 2, float_val );					
		 sprintf(buffer," Az = %s g\t",float_val);			
		 USART_SendString(buffer);						

		 dtostrf( X_g, 3, 2, float_val );					
		 sprintf(buffer," Gx = %s%c/s\t",float_val,0xF8);	
		 USART_SendString(buffer);						
		 dtostrf( Y_g, 3, 2, float_val );					
		 sprintf(buffer," Gy = %s%c/s\t",float_val,0xF8);	
		 USART_SendString(buffer);						
		 
		 dtostrf( Z_g, 3, 2, float_val);					
		 sprintf(buffer," Gz = %s%c/s\r\n",float_val,0xF8);	
		 USART_SendString(buffer);						
		 _delay_ms(1000);								
	 }
 }