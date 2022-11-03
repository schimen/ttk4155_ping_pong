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

int pwm_setup();
void pwm_set_duty_cycle(uint8_t duty_cycle, uint8_t channel);


#endif /* PWM_H_ */
