/*
 * motor.h
 *
 * Created: 08.11.2022 12:47:53
 *  Author: joerg
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "sam.h"
#include <stdio.h>
//#include "util/delay.h"


// MJ1 pins
#define MJ1_NOT_OE      PIO_PD0     // pin 25
#define MJ1_NOT_RST     PIO_PD1     // pin 26
#define MJ1_SEL         PIO_PD2     // pin 27
#define MJ1_EN          PIO_PD9     // pin 30
#define MJ1_DIR         PIO_PD10    // pin 32

// MJ2
#define MJ2_PINS (0x00FF << 1)      // PC1-PC8

enum motor_direction {RIGHT, LEFT};

void motor_setup();
void set_motor_direction(uint8_t direction);

#endif /* MOTOR_H_ */