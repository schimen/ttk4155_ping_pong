#include "pid.h"

void pid_tune(struct pid_t *pid, float K, uint8_t Ti, uint8_t Td, uint8_t N){
	pid->beta = Td/(Td + (PID_SAMPLING_INTERVAL_MS*N)); // beta = Td/(Td + TN)
    pid->Kp = K*GAIN_SCALING;
    pid->Ki = (K*GAIN_SCALING*PID_SAMPLING_INTERVAL_MS)/Ti; // Ki = K*T/Ti
    pid->Kd = K*(Td/PID_SAMPLING_INTERVAL_MS)*GAIN_SCALING;
	pid->sampling_interval = PID_SAMPLING_INTERVAL_MS;
}


int16_t pid_controller(struct pid_t *pid, uint8_t r, uint8_t y){
    // Calculate error
    int16_t e = r - y;
    
    // Map small errors to 0 to avoid oscillations caused by the motor sticking
	if(e < ERROR_TOLERANCE && e > -ERROR_TOLERANCE){ 
		e = 0; 
	}
    
    // Calculate controller terms
    int32_t u_p = pid->Kp * e; // u_p[k] = Kp*e[k]
    int32_t u_i = pid->prev_u_i + pid->Ki * e; // u_i[k] = u_i[k-1] + Ki*e[k]
    int32_t u_d = pid->beta * pid->prev_u_d - pid->Kd*(1 - pid->beta)*(y - pid->prev_y); // u_d[k] = beta*u_d[k-1] - Kd*(1-beta)*(y[k] - y[k-1])

    // Limit integral gain, anti-windup
    if(u_i > MAX_UI){
        u_i = MAX_UI;
    }
    if(u_i < -MAX_UI){
        u_i = -MAX_UI;
    }
    
    // Update previous values in struct
    pid->prev_u_i = u_i;
    pid->prev_u_d = u_d;
    pid->prev_y = y;
	
    // Calculate total output
    int32_t u = u_p + u_i + u_d;

    // Limit total gain
    if(u > MAX_U){
        u = MAX_U;
    }
    if(u < -MAX_U){
        u = -MAX_U;
    }
	
    return (int16_t) (u/GAIN_SCALING);
}