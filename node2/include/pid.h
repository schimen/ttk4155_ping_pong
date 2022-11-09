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


#define MAX_U           12800   // 100% * GAIN_SCALING
#define MAX_UI          6400    // 
#define GAIN_SCALING    128


typedef struct pid_t
{
    uint8_t Kp;     // P-gain, Kp = K
    uint8_t Ki;     // I-gain, Ki = K*(T/Ti)
    uint8_t Kd;     // D-gain, Kd = K*(Td/T)
    uint8_t Ti;     // Integral term time constant
    uint8_t Td;     // Derivative term time constant

    uint8_t sampling_interval;
    uint8_t beta;

    int16_t prev_u_i;
    int16_t prev_u_d; 

    int8_t prev_y;  // previous process value
};


#endif /* PID_H_ */