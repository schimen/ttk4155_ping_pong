/*
 * node2.c
 *
 * Created: 20.10.2022 10:02:51
 * Author : joerg
 */ 
#include <stdbool.h>
#include <stdint.h>
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
GAME_DATA game_data;
PID_DATA pid_data;

uint32_t prevMillis = 0;
uint32_t updateMillis = 0;
uint32_t startTime = 0;
volatile uint16_t score = 0;

volatile bool gameRunning = false;
volatile bool gameOver = false;

enum SCORE_STATE{CURRENT_SCORE, FINAL_SCORE}; 

void LED_setup(void);
void pid_handler(uint8_t setpoint);
void update_score(uint8_t SCORE_STATE);

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
	pid_tune(&pid_data, 0.7, 1000, 0, 10); // K = 0.7, Ti = 1000, Td = 0
	solonoid_setup();
	printf("Node 2 setup done\n\r");
	prevMillis = getMillis();
	
	//encoder_reset();
	
    while (1) 
    {	
		// Wait for user to press "Start game	
		if (game_data.start == 1 && gameRunning == false)
		{
			printf("START GAME\n\r");
			gameRunning = true;
			game_data.start = 0;
			score = 0;
			startTime = getMillis();
			updateMillis = getMillis();
		}
		//Run game
		while(gameRunning)
		{
			if (getMillis() >= prevMillis + PID_SAMPLING_INTERVAL_MS)
			{
				JS_Handler(console_data.dir_joystick);
				pid_handler(console_data.r_slider);
				prevMillis = getMillis();
			}
			//Game ends if ping-pong ball touches lower wall or user presses "Exit"
			if (gameOver || game_data.stop)
			{
				gameOver = false;
				game_data.stop = 0;
				
				printf("GAME OVER\n\r");
				score =  (getMillis() - startTime) / 10000;
				gameRunning = false;
				update_score(FINAL_SCORE);
 			}
			//Send current score to OLED screen on node 1
			else if (getMillis() >= updateMillis + 5000)
			{
				score = (getMillis() - startTime) / 10000;
				update_score(CURRENT_SCORE);
				updateMillis = getMillis();
			}
		}
    } //end while(1)
} //end main


/**
 * @brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 * 
 */
void CAN0_Handler()
{
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	CAN_MESSAGE rx_message;
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&rx_message, 1);
		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		{
			can_receive(&rx_message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
			rx_message.data_length = 0;
		}
		
		// Handle incoming message
		if (rx_message.data_length != 0 && rx_message.id == 1)
		{
			//byte 1: bit 0-2 joystick val, bit 3+4 er knapp R+L
			//byte 2: Left slider
			//byte 3: Right slider
			console_data.dir_joystick = rx_message.data[0] & 0x07;
			console_data.r_button = (rx_message.data[0] >> 3) & 0x01;
			console_data.l_button = (rx_message.data[0] >> 4) & 0x01;
			console_data.l_slider = 100-rx_message.data[1];
			console_data.r_slider = 100-rx_message.data[2];
		}
		else if(rx_message.data_length != 0 && rx_message.id == 2)
		{
			game_data.start = rx_message.data[0];
			game_data.stop = rx_message.data[1];
			printf("start = %d\n\r", game_data.start);
			printf("stop = %d\n\r", game_data.stop);
		}
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
}

/**
 * @brief IR-diode Interrupt handler
 * 
 */
void PIOA_Handler(void) // IR-interrupt
{
	if (((PIOA->PIO_ISR & IR_PIN) == IR_PIN) && gameRunning == true)
	{
			gameOver = true;
	}
}

/**
 * @brief Setup function for on-board LEDS
 * 
 */
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


/**
 * @brief Controller update function. Measures position and sets motor speed and direction based on PID output.
 * 
 * @param setpoint Desired position in range 0-100. 0 is right wall and 100 is left wall.
 */
void pid_handler(uint8_t setpoint){
	// Get motor position
	uint8_t position = encoder_get_position();

	// Get controller output based on setpoint and current position
	int16_t u = pid_controller(&pid_data, setpoint, position);

	// Decide motor direction
	if (u > 0){
		set_motor_direction(MOTOR_LEFT);
	}
	if (u < 0){
		set_motor_direction(MOTOR_RIGHT);
	}
	// Set motor speed
	set_motor_speed(abs(u));
	
}

/**
 * @brief Sends the current and final score to Node 1. Resets score if the game has ended.
 * 
 * @param SCORE_STATE enum holding CURRENT_SCORE and FINAL_SCORE, depending on game state
 */
void update_score(uint8_t SCORE_STATE)
{
	if(CAN0->CAN_SR & CAN_SR_MB0) //CAN0 MB0 ready to send 
	{
		CAN_MESSAGE tx_message;
		
		CAN0->CAN_IDR = CAN_IER_MB0; //Disable interrupt
		tx_message.id = 2;
		tx_message.data_length = 3;
		tx_message.data[0] = 0; //Start game
		tx_message.data[2] = score;
		
		switch (SCORE_STATE) {
		case CURRENT_SCORE:
			tx_message.data[1] = 0; //Stop game
			break;
		case FINAL_SCORE:
			tx_message.data[1] = 1; //Stop game
			gameRunning = false;
			score = 0;
			break;
		}
		can_send(&tx_message,0);
	}
}