/*
 * pid.c
 *
 * Created: 07.11.2022 15:50:31
 *  Author: joerg
 */ 

#include "pid.h"

void setup_pid(struct pid_t *pid, uint8_t K, uint8_t Ti, uint8_t Td, uint8_t N, uint8_t T){
    // tune pid based on user input
    tune_pid(&pid, K, Ti, Td, N, T);
}

void tune_pid(struct pid_t *pid, uint8_t K, uint8_t Ti, uint8_t Td, uint8_t N, uint8_t T){
    // calculate new controller values based on input and update the pid struct
    pid->alpha = T/Ti;
    pid->beta = Td/(Td + (T*N)); // beta = Td/(Td + TN)

    pid->Kp = K;
    pid->Ki = K*pid->alpha; // Ki = K*T/Ti
    pid->Kd = K*(Td/T);

    pid->sampling_interval = T;

}


int16_t pid_gains(struct pid_t *pid){
    // Calculate controller gains
    int16_t u_p = pid->Kp * pid->e;
    int16_t u_i = pid->u_i[1] + pid->Ki * pid->e; // u_i[n] = u[n-1] + Ki*a*e[n]
    int16_t u_d = pid->beta * pid->u_d[1] - pid->Kd*(1 - pid->beta)*(pid->y[0] - pid->y[1]); //

    // limit integral gain
    if(u_i > MAX_UI){
        u_i = MAX_UI;
    }
    if(u_i < -MAX_UI){
        u_i = -MAX_UI;
    }


    // update pid struct gains
    pid->u_p = u_p;
    pid->u_i[1] = pid->u_i[0];
    pid->u_i[0] = u_i;
    pid->u_d[1] = pid->u_d[0];
    pid->u_d[0] = u_d;

    return u_p + u_i + u_d;
}