/*
 * node2.c
 *
 * Created: 20.10.2022 10:02:51
 * Author : joerg
 */ 
#include <stdbool.h>
#include "sam.h"
#include "uart.h"
#include "string.h"
#include "printf_stdarg.h"
#include "can_controller.h"
#include "console_lib.h"
#include "pwm_lib.h"
#include "systick_lib.h"
#include "motor.h"
#include "pid.h"

#define DEBUG_INTERRUPT 0
#define LED0_PIN PIO_PA19 
#define LED1_PIN PIO_PA20



CONSOLE_DATA console_data;
PID_DATA pid_data;

uint32_t prevMillis = 0;
uint32_t startTime = 0;

volatile bool gameRunning = false;
volatile bool gameOver = false;

void LED_setup(void);

void pid_handler(uint8_t setpoint);

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable watchdogtimer
	configure_uart();
	printf("starting setup\n\r");
	SysTick_Config(10500); //1ms in between ticks
	can_setup();
	LED_setup();
	pwm_setup();
	ir_setup();
	motor_setup();
	pid_tune(&pid_data, 1, 100, 0, 10);
	solonoid_setup();
	printf("Node 2 setup done\n\r");
	prevMillis = getMillis();
	
	//encoder_reset();
	
    while (1) 
    {		
		if (console_data.r_button && gameRunning == false)
		{
			printf("START GAME\n\r");
			gameRunning = true;
			startTime = getMillis();
		}
		
		while(gameRunning)
		{
			if (getMillis() >= prevMillis + PID_SAMPLING_INTERVAL_MS)
			{
				JS_Handler(console_data.dir_joystick);
				pid_handler(console_data.l_slider);
				prevMillis = getMillis();
			}
			if (gameOver)
			{
				uint16_t score =  (getMillis() - startTime) / 1000;
				printf("Your score: %d \n\r", score);
				gameRunning = false;
				gameOver = false;
			}
		}
    } //end while(1)
} //end main


/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler()
{
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	CAN_MESSAGE message;
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);
		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
			message.data_length = 0;
		}
		
		/* Handle incoming message */
		
		if (message.data_length != 0 && message.id == 1)
		{
			//byte 1: bit 0-2 joystick val, bit 3+4 er knapp R+L
			//byte 2: Left slider
			//byte 3: Right slider
			console_data.dir_joystick = message.data[0] & 0x07;
			console_data.r_button = (message.data[0] >> 3) & 0x01;
			console_data.l_button = (message.data[0] >> 4) & 0x01;
			console_data.l_slider = 100-message.data[1];
			console_data.r_slider = message.data[2];
		
			if(DEBUG_INTERRUPT)printf("Console values: \n\r");
			if(DEBUG_INTERRUPT)printf("dir_joystick: %d \n\r", console_data.dir_joystick);
			if(DEBUG_INTERRUPT)printf("L_btn: %d \n\r", console_data.l_button);
			if(DEBUG_INTERRUPT)printf("R_btn: %d \n\r", console_data.r_button);
			if(DEBUG_INTERRUPT)printf("L_slider: %d \n\r", console_data.l_slider);
			if(DEBUG_INTERRUPT)printf("R_slider: %d \n\r", console_data.r_slider);
		}
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		//if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;
	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");
	}
	
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");
	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}

void PIOA_Handler(void)
{
	if (((PIOA->PIO_ISR & IR_PIN) == IR_PIN) && gameRunning == true)
	{
			printf("GAME OVER\n\r");
			gameOver = true;
	}
}

void LED_setup(void)
{
		// PORT enable IO
		PIOA->PIO_PER |= (LED0_PIN | LED1_PIN);
		// PORT pin set direction as output
		PIOA->PIO_OER |= (LED0_PIN | LED1_PIN);
		// PORT pin disable internal pullup
		PIOA->PIO_PUDR |= (LED0_PIN | LED1_PIN);
		// enable LED io to be written directly
		PIOA->PIO_OWER = (LED0_PIN | LED1_PIN);
		//Turn on LEDs
		PIOA->PIO_ODSR |= LED0_PIN;
		PIOA->PIO_ODSR |= LED1_PIN;
}

void pid_handler(uint8_t setpoint){
	// Measure motor position
	uint8_t position = encoder_get_position();
	//printf("position: %d \n\r", position);
	int16_t u = pid_controller(&pid_data, setpoint, position);
	if (u > 100) { u = 100;}
	if (u < -100) { u = -100;}
	pid_data.u = u;
	
	if (u > 0){
		set_motor_direction(MOTOR_LEFT);
	}
	if (u < 0){
		set_motor_direction(MOTOR_RIGHT);
	}
	set_motor_speed(u);
}