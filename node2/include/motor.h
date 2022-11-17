#ifndef MOTOR_H_
#define MOTOR_H_

#include "sam.h"
#include <stdio.h>
#include "systick_lib.h"

/**
 * @brief Motor control API for node 2
 * @defgroup motor
 * @ingroup motor
 * 
*/

/**
 * @brief Definition of header pins used to interface with the MJ1 connector on
 * the motor box
 * @defgroup motor_MJ1
 * @ingroup motor
 */

/** !OE pin. Encoder output enable */
#define MJ1_NOT_OE      PIO_PD0
/** !RST pin. Encoder reset */
#define MJ1_NOT_RST     PIO_PD1     
/** SEL pin. Encoder select low/high byte */
#define MJ1_SEL         PIO_PD2
/** EN pin. Motor enable */
#define MJ1_EN          PIO_PD9
/** DIR pin. Motor direction */
#define MJ1_DIR         PIO_PD10
/** @} */


/** @brief Mask used to select MJ2 pins on the motorbox. PC1-PC8 on the ATSAM3x8E */
#define MJ2_PINS 0x01FE

/** @brief Max encoder value. Assigned in calibration function */
uint16_t encoder_max;

/** @brief Used to set motor direction */
enum motor_direction {MOTOR_RIGHT, MOTOR_LEFT};

/**
 * @brief Setup of shield interface with external motorbox, i.e. DAC, MJ1 and MJ2 pins.
 * 
 */
void motor_setup();

/**
 * @brief Set the motor direction
 * 
 * @param direction Number of type motor_direction
 */
void set_motor_direction(uint8_t direction);

/**
 * @brief Set the motor speed
 * 
 * @param speed Speed in range 0-100
 */
void set_motor_speed(uint8_t speed);

/**
 * @brief Read the motorbox encoder
 * 
 * @return Raw encoder value
 */
int16_t encoder_read();

/**
 * @brief Get the motor position. 0 is right wall, 100 is left wall.
 * 
 * @return Position in range 0-100
 */
uint8_t encoder_get_position();

/**
 * @brief Reset motorbox encoder
 * 
 */
void encoder_reset();


/**
 * @brief Calibrate motorbox encoder. 
 * 
 */
void encoder_calibrate();


/** @} */

#endif /* MOTOR_H_ */