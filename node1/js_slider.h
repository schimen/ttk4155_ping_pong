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
#include "util/delay.h"
#include "adc_lib.h"

/* PRIVATE FUNCTIONS */
/* ADC-value to direction */
uint8_t get_JS_position(uint8_t x_val, uint8_t y_val);

/* ADC-value to percentage */
uint8_t get_slider_position(uint8_t val);

/* PUBLIC FUNCTIONS */
/* Converts joystick value from ADC to a direction and prints */
void JS_service(void);

/* Converts slider value from ADC to percentage and prints */
void silder_service(struct ADC_Values *adc_val);

#endif