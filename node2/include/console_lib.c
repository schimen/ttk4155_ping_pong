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
		currentServoPos = 90;
		servo_set_pos(currentServoPos,6);
		break;
		case RIGHT:
		if (currentServoPos > 0)
		{
			currentServoPos--;
			servo_set_pos(currentServoPos, 6);
			
		}
		break;
		case LEFT:
		if (currentServoPos < 180)
		{
			currentServoPos++;
			servo_set_pos(currentServoPos, 6);
			
		}
		break;
		case UP:
		PIOA->PIO_CODR = SOLO_PIN; //BOUNCE!
		ms_delay(SOLO_DELAY_MS);
		PIOA->PIO_SODR = SOLO_PIN;
		break;
		case DOWN:
		printf("DOWN\n\r");
		break;
	}
}
/*
* Setup of the infrared sensor
*/
void ir_setup(){
	// Enable pin
	PIOA->PIO_PER |= IR_PIN;
	while((PIOA->PIO_PSR & IR_PIN) == 0);
	// Set as input
	PIOA->PIO_ODR |= IR_PIN;
	// Disable pull-up
	PIOA->PIO_PUDR = IR_PIN;
	// Enable PMC clock
	PMC->PMC_PCER0 |= PMC_PCER0_PID11;
	// Enable interrupt in NVIC
	NVIC_EnableIRQ(PIOA_IRQn); 
	// Make sure falling edge is used
	PIOA->PIO_AIMER = IR_PIN;
	// Edge detection interrupt enable
	PIOA->PIO_ESR = IR_PIN;
	// Falling edge
	PIOA->PIO_FELLSR = IR_PIN;
	
	// Clock divider for debounce: t_div = 2*(div+1)*t_slow_clock (~31 microsec)
	PIOA->PIO_SCDR = 0xFF; // Clock divider for 0.25 s debounce
	// Enable input glitch and debounce filter
	PIOA->PIO_DIFSR |= IR_PIN;
	if ((REG_PIOA_IFDGSR & IR_PIN) == 0) {
		printf("Could not set debounce filter on IR pin\n\r");
	}
	PIOA->PIO_IFER |= IR_PIN;
	
	if (PIOA->PIO_ELSR & IR_PIN) {
		printf("Error: level interrupt selected for IR pin\n\r");
	} 
	// Enable interrupt on pin
	PIOA->PIO_IER = IR_PIN;
}

void solonoid_setup()
{
	// Enable PMC clock
	PMC->PMC_PCER0 |= PMC_PCER0_PID11;
	// PORT enable IO
	PIOA->PIO_PER |= SOLO_PIN;
	// PORT pin set direction as output
	PIOA->PIO_OER |= SOLO_PIN;
	// PORT pin enable internal pullup
	PIOA->PIO_PUDR &= ~(SOLO_PIN);
	// Enable pin io to be written directly
	PIOA->PIO_OWER = (SOLO_PIN);
	//Set pin high
	PIOA->PIO_ODSR |= SOLO_PIN;
}