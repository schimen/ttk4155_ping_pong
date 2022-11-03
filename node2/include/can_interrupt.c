/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"
#include <stdio.h>
#include "sam.h"
#include "printf_stdarg.h"
#include "can_controller.h"
#include "console_lib.h"


#define DEBUG_INTERRUPT 1

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
		CONSOLE_DATA console_data;
		if (message.data_length != 0)
		{
			//byte 1: bit 0-2 joystick val, bit 3+4 er knapp R+L
			//byte 2: Left slider
			//byte 3: Right slider
			if (message.id == 1)
			{
				console_data.dir_joystick = message.data[0] & 0x07;
				console_data.r_button = (message.data[0] >> 3) & 0x01;
				console_data.l_button = (message.data[0] >> 4) & 0x01;
				console_data.l_slider = message.data[1]; 
				console_data.r_slider = message.data[2];
			}
		
		if(DEBUG_INTERRUPT)printf("Console values: \n\r");
		if(DEBUG_INTERRUPT)printf("dir_joystick: %d \n\r", console_data.dir_joystick);
		if(DEBUG_INTERRUPT)printf("L_btn: %d \n\r", console_data.l_button);
		if(DEBUG_INTERRUPT)printf("R_btn: %d \n\r", console_data.r_button);
		if(DEBUG_INTERRUPT)printf("L_slider: %d \n\r", console_data.l_slider);
		if(DEBUG_INTERRUPT)printf("R_slider: %d \n\r", console_data.r_slider);
		}
		
		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
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
