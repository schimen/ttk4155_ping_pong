/*
 * pid.h
 *
 * Created: 07.11.2022 15:50:13
 *  Author: joerg
 */ 


#ifndef PID_H_
#define PID_H_


#include <stdio.h>
#include "sam.h"
#include "console_lib.h"
#include "motor.h"


#define MAX_U           51200   // 100% * GAIN_SCALING
#define MAX_UI          15000    // 
#define GAIN_SCALING    512

#define PID_SAMPLING_INTERVAL_MS 10

typedef struct pid_t
{
    uint32_t Kp;     // P-gain, Kp = K
    uint32_t Ki;     // I-gain, Ki = K*(T/Ti)
    uint32_t Kd;     // D-gain, Kd = K*(Td/T)
    uint16_t Ti;     // Integral term time constant
    uint16_t Td;     // Derivative term time constant

    uint8_t sampling_interval;
    uint8_t beta;
	
	uint32_t u; // Controller output
	
    int32_t prev_u_i;
    int32_t prev_u_d; 

    int8_t prev_y;  // previous process value
} PID_DATA;


void pid_setup(struct pid_t *pid);

void pid_tune(struct pid_t *pid, float K, uint8_t Ti, uint8_t Td, uint8_t N);

int16_t pid_controller(struct pid_t *pid, uint8_t r, uint8_t y);



#endif /* PID_H_ */