#ifndef JS_SLIDER_H_
#define JS_SLIDER_H_

/**
 * @brief Joystick and slider API for node 1
 * @defgroup js_slider
 * @ingroup js_slider
 * @{
 */

#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include "util/delay.h"
#include "interrupt.h"

/** External oscillator frequency */
#define F_CPU 4915200

/** Joystick direction enum */
enum JS_position {DEFAULT, RIGHT, LEFT, UP, DOWN};

/** Struct holding current joystick values */
struct Joystick_t {
	uint8_t  x_adc; // x-value from ADC
	uint8_t  y_adc; // y-value from ADC
	uint8_t x_default;
	uint8_t y_default;
	int8_t  x_pos;	 // x-value in percent
	int8_t  y_pos;	 // y-value in percent
	uint8_t direction; // Direction of joystick (JS position)
};


/** Struct holding current slider values */
struct Slider_t {
	uint8_t  left_adc;	// Left slider from ADC
	uint8_t  right_adc;	// Right slider from ADC
	uint8_t  left_pos;	// Left slider in percent
	uint8_t  right_pos;	// Right slider in percent
};


/**
 * @brief Start address for ADC read 
 * 
 */
#define ADC_START_ADR 0x1400


/**
 * @brief Update joystick and slider values from ADC 
 * 
 */
void js_slider_update(void);

/**
 * @brief Averages X amount of adc-measurements to set the default adc-values for the joystick in default position
 * 
 */
void calibrate_joystick(void);

/**
 * @brief Return true if there is a new value at left slider
 * 
 * @return True if new value available
 */
bool new_slider_left();

/**
 * @brief Return true if there is a new value at right slider
 * 
 * @return True if new value avaible
 */
bool new_slider_right();

/**
 * @brief  Return true if there is a new direction at joystick
 * 
 * @return True if new value available
 */
bool new_joystick_direction(void);


/**
 * @brief Return left slider position
 * 
 * @return uint8_t left slider position
 */
uint8_t left_slider_pos();


/**
 * @brief Return right slider position
 * 
 * @return uint8_t right slider position
 */
uint8_t right_slider_pos();


/**
 * @brief Return joystick direction
 * 
 * @return uint8_t joystick direction
 */
uint8_t joystick_direction();


/**
 * @brief Return true if something has changed from input
 * 
 * @param left_btn_pressed Bool representing left touch button
 * @param right_button_pressed Bool representing right touch button
 * @return True if there is a change in the data from the multifunction USB board 
 */
bool new_event(bool left_btn_pressed, bool right_button_pressed);


/**
 * @brief Write event data to tx buffer, used for sending over CAN
 * 
 * @param buffer Data to transmit
 * @param left_btn_pressed Bool representing left touch button
 * @param right_btn_pressed Bool representing right touch button
 */
void write_event_data(uint8_t *buffer, bool left_btn_pressed, bool right_btn_pressed);

/** @} */

#endif