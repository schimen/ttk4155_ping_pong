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
#include "systick_lib.h"


// MJ1 pins
#define MJ1_NOT_OE      PIO_PD0     // pin 25
#define MJ1_NOT_RST     PIO_PD1     // pin 26
#define MJ1_SEL         PIO_PD2     // pin 27
#define MJ1_EN          PIO_PD9     // pin 30
#define MJ1_DIR         PIO_PD10    // pin 32

// MJ2
//#define MJ2_PINS (0x00FF << 1)      // PC1-PC8
#define MJ2_PINS 0x01FE

#define ENCODER_MAX 8100
#define MOTOR_TRESHOLD 40

enum motor_direction {MOTOR_RIGHT, MOTOR_LEFT};

void motor_setup();

void set_motor_direction(uint8_t direction);

void set_motor_speed(uint8_t speed);

int16_t encoder_read();

uint8_t encoder_get_position();

void encoder_reset();

void encoder_calibrate();



#endif /* MOTOR_H_ */