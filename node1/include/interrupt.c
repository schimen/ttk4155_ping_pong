/*
 * interrupt.c
 *
 * Created: 20.10.2022 11:16:07
 *  Author: lineh
 */ 
#include "interrupt.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>


/* Setup for USB-board touch-buttons with interrupt on rising edge and CAN msg on falling edge */
void interrupt_setup(void)
{
	// Set button pins as input
	DDRD &= ~((1 << LEFT_BUTTON) | (1 << RIGHT_BUTTON));
	DDRE &= ~(1 << JS_BUTTON);
	
	// Set internal pull-up for JS-button
	PORTE |= (1 << JS_BUTTON);
	
	// INT0 and INT1 triggers on rising edge, INT2 triggers on falling edge
	MCUCR |= 0x0F;
	
	// Enable INT0, INT1 and INT2
	GICR = ((1 << INT0) | (1 << INT1) | (1 << INT2));
}
