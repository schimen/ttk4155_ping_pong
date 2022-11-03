/*
 * joystick.h
 *
 * Created: 27.10.2022 14:58:50
 *  Author: lineh
 */ 


#ifndef CONSOLE_LIB_H_
#define CONSOLE_LIB_H_
#include <stdio.h>
#include "pwm_lib.h"
#include "systick_lib.h"

enum JS_direction {DEFAULT, RIGHT, LEFT, UP, DOWN};
	
typedef struct console_data_t
{
	uint8_t dir_joystick;
	uint8_t l_button;
	uint8_t r_button;
	uint8_t l_slider;
	uint8_t r_slider;
} CONSOLE_DATA;

uint8_t currentServoPos;

void JS_Handler(uint8_t direction);




#endif /* CONSOLE_LIB_H_ */