/*
 * pwm.h
 *
 * Created: 27.10.2022 15:13:02
 *  Author: joerg
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "sam.h"
#include <stdio.h>

/**
 * @brief Set up PWM on pin PC18, PWM channel 6H
 * 
 * @return int 0 if succesfull, negative value on error
 */
int pwm_setup();

/**
 * @brief Set duty cycle for pwm
 * 
 * @param duty_cycle Unsigned number between 0 and 100 
 *   representing percentage of duty-cycle to set
 * @param channel PWM channel
 */
void pwm_set_duty_cycle(uint8_t duty_cycle, uint8_t channel);


/**
 * @brief Set servo position
 * 
 * @param pos Unsigned number between 0 and 180 representing servo position
 * @param channel PWM channel
 */
void servo_set_pos(uint8_t pos, uint8_t channel);

#endif /* PWM_H_ */
