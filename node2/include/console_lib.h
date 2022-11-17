#ifndef CONSOLE_LIB_H_
#define CONSOLE_LIB_H_

/**
 * @brief Drivers for handling IR sensor, solenoid actuation and incoming multifunction USB board data
 * @defgroup console
 * @ingroup console
 * @{
 */

#include <stdio.h>
#include "sam.h"
#include "pwm_lib.h"
#include "systick_lib.h"


/** Infrared sensor pin */
#define IR_PIN PIO_PA14
/** Solenoid relay pin */
#define SOLO_PIN PIO_PA15
/** Solenoid actuation delay */
#define SOLO_DELAY_MS 300

/** Joystick direction enum */
enum JS_direction {DEFAULT, RIGHT, LEFT, UP, DOWN};
	
/** Struct to keep current readings of joystick and buttons from node 1 */
typedef struct console_data_t
{
	uint8_t dir_joystick;
	uint8_t l_button;
	uint8_t r_button;
	uint8_t l_slider;
	uint8_t r_slider;
} CONSOLE_DATA;

/** Struct to keep current game status */
typedef struct game_data_t
{
	uint8_t start;
	volatile uint8_t stop;
	uint8_t score;
	
} GAME_DATA;

/** Current servo position in the range 0-180 */
uint8_t currentServoPos;

/**
 * @brief Actuates servo and solenoid based on Joystick readings from node 1
 * 
 * @param direction Joystick direction from node 1, type JS_direction.
 */
void JS_Handler(uint8_t direction);


/**
 * @brief Sets up the infrared sensor pin and interrupts
 * 
 */
void ir_setup();


/**
 * @brief Sets up the solenoid relay pins
 * 
 */
void solonoid_setup();

/** @} */

#endif /* CONSOLE_LIB_H_ */