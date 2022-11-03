/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_
#include "console_lib.h"

typedef struct console_data_t
{
	uint8_t dir_joystick;
	uint8_t l_button;
	uint8_t r_button;
	uint8_t l_slider;
	uint8_t r_slider;
} CONSOLE_DATA;

void CAN0_Handler(void);






#endif /* CAN_INTERRUPT_H_ */