#ifndef PID_H_
#define PID_H_


#include <stdio.h>
#include "sam.h"
#include "console_lib.h"
#include "motor.h"

/**
 * @brief PID controller API for node 2
 * @defgroup pid
 * @ingroup pid
 */

/** PID parameter scaling factor. Used to get higher resolution parameters */
#define GAIN_SCALING    512 
/** PID output upper bound. Equal to 100*GAIN_SCALING */
#define MAX_U           51200
/** Integral term output upper bound. Serves as anti-windup. */
#define MAX_UI          15000
/** Error tolerance. Errors within this parameter are mapped to 0 to avoid oscillations caused by the motor sticking */
#define ERROR_TOLERANCE 5
/** Sampling interval of the controller */
#define PID_SAMPLING_INTERVAL_MS 1


/**
 * @brief Struct containing PID parameters
 * 
 */
typedef struct pid_t
{
    uint32_t Kp;     // P-gain, Kp = K
    uint32_t Ki;     // I-gain, Ki = K*(T/Ti)
    uint32_t Kd;     // D-gain, Kd = K*(Td/T)
    uint16_t Ti;     // Integral term time constant
    uint16_t Td;     // Derivative term time constant

    uint8_t sampling_interval;
    uint8_t beta;       // Derivative term scaling factor
	
	uint32_t u;         // Controller output
	
    int32_t prev_u_i;   // Previous integral term
    int32_t prev_u_d;   // Previous derivative term

    int8_t prev_y;      // previous process value
} PID_DATA;


/**
 * @brief Calculates and changes the controller parameters in the passed struct
 * 
 * @param pid Pointer to struct of type pid_t that contains the pid parameters
 * @param K Controller gain
 * @param Ti Integral time constant
 * @param Td Derivative time constant 
 * @param N Derivative filter constant
 */
void pid_tune(struct pid_t *pid, float K, uint8_t Ti, uint8_t Td, uint8_t N);


/**
 * @brief Calculates PID controller output
 * 
 * @param pid Pointer to struct of type pid_t that contains the pid parameters  
 * @param r Setpoint
 * @param y Process value
 * @return Controller output
 */
int16_t pid_controller(struct pid_t *pid, uint8_t r, uint8_t y);


/** @} */

#endif /* PID_H_ */