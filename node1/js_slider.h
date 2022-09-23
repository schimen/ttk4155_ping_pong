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
#include "adc_lib.h"

#define LEFT_BUTTON PIND2 //INT0
#define RIGHT_BUTTON PIND3 //INT1

//Flags for detecting button presses
volatile bool left_btn_pressed; 
volatile bool right_btn_pressed;

struct Joystick_t {
	uint8_t  x_adc; // x-value from ADC
	uint8_t  y_adc; // y-value from ADC
	uint8_t x_default;
	uint8_t y_default;
	int8_t  x_pos;	 // x-value in percent
	int8_t  y_pos;	 // y-value in percent
}joystick;

struct Slider_t {
	uint8_t  left_adc;	// Left slider from ADC
	uint8_t  right_adc;	// Right slider from ADC
	uint8_t  left_pos;	// Left slider in percent
	uint8_t  right_pos;	// Right slider in percent
}slider;


/* Averages X amount of adc-measurements to set the default adc-values for the joystick in default position */
void calibrate_joystick(void);

/* Converts a adc-value of the joystick to a percentage*/
int8_t get_JS_position(uint8_t adc_val, uint8_t default_val);

/* Converts a percent-value of the joystick to a direction */
uint8_t get_JS_direction(int8_t x_val, int8_t y_val);

/* Silder ADC-value to percentage */
uint8_t get_slider_position(uint8_t val);

/* Setup for USB-board touch-buttons with interrupt on rising edge */
void button_setup(void);

/*------------- PLACEHOLDER FUNCTIONS -------------*/
/* Placeholder function for joystick: Converts joystick value from ADC to a direction and prints it */
void JS_service(void);

/* Placeholder function for sliders: Converts slider value from ADC to percentage and prints it */
void silder_service(void);

/* Placeholder function buttons: Prints when buttons are pushed */
void button_service(void);

#endif