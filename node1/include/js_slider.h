/*
 * js_slider.h
 *
 * Created: 19.09.2022 18:00:41
 *  Author: lineh
 */ 
#ifndef JS_SLIDER_H_
#define JS_SLIDER_H_

#define F_CPU 4915200
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include "util/delay.h"
#include "interrupt.h"

enum JS_position {DEFAULT, RIGHT, LEFT, UP, DOWN};

struct Joystick_t {
	uint8_t  x_adc; // x-value from ADC
	uint8_t  y_adc; // y-value from ADC
	uint8_t x_default;
	uint8_t y_default;
	int8_t  x_pos;	 // x-value in percent
	int8_t  y_pos;	 // y-value in percent
	uint8_t direction; // Direction of joystick (JS position)
};

struct Slider_t {
	uint8_t  left_adc;	// Left slider from ADC
	uint8_t  right_adc;	// Right slider from ADC
	uint8_t  left_pos;	// Left slider in percent
	uint8_t  right_pos;	// Right slider in percent
};

// Start address for ADC read
#define ADC_START_ADR 0x1400

/* Update joystick and slider values from ADC */
void js_slider_update(void);

/* Averages X amount of adc-measurements to set the default adc-values for the joystick in default position */
void calibrate_joystick(void);

/* Return true if there is a new value at left slider */
bool new_slider_left();

/* Return true if there is a new value at right slider */
bool new_slider_right();

/* Return true if there is a new direction at joystick */
bool new_joystick_direction(void);

/* Return left slider position */
uint8_t left_slider_pos();

/* Return right slider position */
uint8_t right_slider_pos();

/* Return joystick direction */
uint8_t joystick_direction();

/* Return true if something has changed from input */
bool new_event(bool left_btn_pressed, bool right_button_pressed);

/* Write event data to tx buffer, used for sending over CAN */
void write_event_data(uint8_t *buffer, bool left_btn_pressed, bool right_btn_pressed);

#endif