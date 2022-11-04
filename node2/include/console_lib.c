/*
 * joystick.c
 *
 * Created: 27.10.2022 14:58:34
 *  Author: lineh
 */ 
#include "console_lib.h"

void JS_Handler(uint8_t direction)
{	
	switch (direction)
	{
		case DEFAULT:
		//printf("DEFAULT\n\r");
		break;
		case RIGHT:
		if (currentServoPos > 0)
		{
			currentServoPos--;
			servo_set_pos(currentServoPos, 6);
			ms_delay(20);
		}
		//printf("RIGHT\n\r");
		break;
		case LEFT:
		if (currentServoPos < 180)
		{
			currentServoPos++;
			servo_set_pos(currentServoPos, 6);
			ms_delay(20);
		}
		//printf("LEFT\n\r");
		break;
		case UP:
		printf("UP\n\r");
		break;
		case DOWN:
		printf("DOWN\n\r");
		break;
	}
}

void ir_setup(){
	// Setup of the infrared sensor
	// Enable PMC clock
	PMC->PMC_PCER0 |= PMC_PCER0_PID11;
	// Enable pin
	PIOA->PIO_PER |= IR_PIN;
	while((PIOA->PIO_PSR & IR_PIN) == 0);
	// Disable output
	PIOA->PIO_ODR |= IR_PIN;
	// Enable input glitch and debounce filter
	PIOA->PIO_IFER |= IR_PIN;
	PIOA->PIO_DIFSR |= IR_PIN;
	// Enable interrupt on pin
	PIOA->PIO_IER |= IR_PIN;
	PIOA->PIO_ESR |= IR_PIN; // Edge detection interrupt enable
	PIOA->PIO_FELLSR |= IR_PIN; // Falling edge
	
	NVIC_EnableIRQ(PIOA_IRQn); // Enable interrupt in NVIC
}


void PIOA_Handler(void){
	printf("IR test!!!\n\r");
}
