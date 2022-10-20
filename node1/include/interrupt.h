/*
 * interrupt.h
 *
 * Created: 20.10.2022 11:16:28
 *  Author: lineh
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define LEFT_BUTTON PIND2 //INT0
#define RIGHT_BUTTON PIND3 //INT1
#define JS_BUTTON PINE0 //INT2

//Flags for detecting button presses
volatile bool left_btn_pressed;
volatile bool right_btn_pressed;
volatile bool JS_btn_pressed;

void interrupt_setup(void);

#endif /* INTERRUPT_H_ */