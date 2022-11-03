/*
 * joystick.h
 *
 * Created: 27.10.2022 14:58:50
 *  Author: lineh
 */ 


#ifndef CONSOLE_LIB_H_
#define CONSOLE_LIB_H_
#include <stdio.h>
#include "sam.h"

#define IR_PIN PIO_PA14

enum JS_direction {DEFAULT, RIGHT, LEFT, UP, DOWN};

uint8_t lastDirection;

void JS_Handler(uint8_t direction);

void ir_setup();





#endif /* CONSOLE_LIB_H_ */