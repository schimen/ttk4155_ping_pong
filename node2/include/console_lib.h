/*
 * joystick.h
 *
 * Created: 27.10.2022 14:58:50
 *  Author: lineh
 */ 


#ifndef CONSOLE_LIB_H_
#define CONSOLE_LIB_H_
#include <stdio.h>

enum JS_direction {DEFAULT, RIGHT, LEFT, UP, DOWN};



void JS_Handler(uint8_t direction);




#endif /* CONSOLE_LIB_H_ */