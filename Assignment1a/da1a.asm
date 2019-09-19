;
; jebA1.asm
;
; Created: 9/18/2019 6:36:44 PM
; Author : marinj4
;


	LDI R25,0x11		;holds the first high bit values of the 16 bit multiplicand 
	LDI R24,0x11		;holds the first low bit values of the the 16 bit multiplicand

	;Multiplicand holds the value of 4369
	
	LDI R23,0x01		;holds the first high bit valules of the 16 bit multiplier	
	LDI R22,0x01		;holds the first low bit values of the 16 bit multiplier
	
	;Multiplier holds the value of 257

	LDI R20,0			;initializes the register to 0	
	LDI R19,0			;initializes the register to 0
	LDI R18,0			;initializes the register to 0
	LDI R17,0			;initializes the register to 0
	LDI R16,0			;initializes the register to 0
	LDI R21,0			;initializes the register to 0

	INC R22
	
Loop1:					;loop that decrements the value of the multplier
						;and adds the value of the multiplicand by that many times
						;for R22

	ADD R17,R24			;holds the last 8 bit result
	ADC R18,R25			;holds the second to the last 8 bits of the result with the carry
	ADC R19,R16			;holds the second highest 8 bits of the result with the carry
	ADC R20,R21			;holds the highest 8 bits of the result with the carry 
	DEC R22				;decrements the multiplier
	BRNE LOOP1			;checks if the multiplier is 0 if not then keep looping
						;till the multiplier is 0
	COM	R23				;ones complements R23 
	INC R23
	
Loop2:					;loop that decrements the value of the multplier
						;and adds the value of the multiplicand by that many times
						;For R23

	ADD R17,R24			;holds the last 8 bit result
	ADC R18,R25			;holds the second to the last 8 bits of the result with the carry
	ADC R19,R16			;holds the second highest 8 bits of the result with the carry
	ADC R20,R21			;holds the highest 8 bits of the result with the carry 
	DEC R23				;decrements the multiplier
	BRNE LOOP2			;checks if the multiplier is 0 if not then keep looping
						;till the multiplier is 0
						
END: JMP END						
						
						
						