/*
 * pid.c
 *
 * Created: 07.11.2022 15:50:31
 *  Author: joerg
 */ 

#include "pid.h"

void pid_setup(struct pid_t *pid){
    // tune pid
}

void pid_tune(struct pid_t *pid, float K, uint8_t Ti, uint8_t Td, uint8_t N){
    // calculate new controller values based on input and update the pid struct
	pid->beta = Td/(Td + (PID_SAMPLING_INTERVAL_MS*N)); // beta = Td/(Td + TN)
    pid->Kp = K*GAIN_SCALING;
    pid->Ki = (K*GAIN_SCALING*PID_SAMPLING_INTERVAL_MS)/Ti; // Ki = K*T/Ti
    pid->Kd = K*(Td/PID_SAMPLING_INTERVAL_MS)*GAIN_SCALING;
	pid->sampling_interval = PID_SAMPLING_INTERVAL_MS;
	
	printf("Kp = %d \n\r", pid->Kp);
	printf("Ki = %d \n\r", pid->Ki);
	printf("Kd = %d \n\r", pid->Kd);
}


int16_t pid_controller(struct pid_t *pid, uint8_t r, uint8_t y){
    /* Outputs a gain u in range [-100, 100] based on pid struct parameters, a setpoint and a position */

    // Calculate error
    int16_t e = r - y;
	//printf("e: %d \n\r", e);
	if(e < 5 && e > -5){ 
		e = 0; 
	}
    
	//printf("e: %d \n\r", e);
	
    // Calculate controller gains
    int32_t u_p = pid->Kp * e;
    int32_t u_i = pid->prev_u_i + pid->Ki * e; // u_i[k] = u_i[k-1] + Ki*e[k]
    int32_t u_d = pid->beta * pid->prev_u_d - pid->Kd*(1 - pid->beta)*(y - pid->prev_y); // u_d[k] = beta*u_d[k-1] - Kd*(1-beta)*(y[k] - y[k-1])

    // limit integral gain, anti-windup
    if(u_i > MAX_UI){
        u_i = MAX_UI;
    }
    if(u_i < -MAX_UI){
        u_i = -MAX_UI;
    }

    // update pid struct gains
    pid->prev_u_i = u_i;
    pid->prev_u_d = u_d;
    pid->prev_y = y;
	
	//printf("ui: %d\n\r", u_i);
    int32_t u = u_p + u_i + u_d;
    
    // limit total gain
    if(u > MAX_U){
        u = MAX_U;
    }
    if(u < -MAX_U){
        u = -MAX_U;
    }
	
    return (int16_t) (u/GAIN_SCALING);
}