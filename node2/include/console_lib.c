/*
 * joystick.c
 *
 * Created: 27.10.2022 14:58:34
 *  Author: lineh
 */ 
#include "console_lib.h"

void JS_Handler(uint8_t direction)
{	
	if (direction != lastDirection)
	{
		switch (direction)
		{
			case DEFAULT:
			printf("DEFAULT\n\r");
			break;
			case RIGHT:
			printf("RIGHT\n\r");
			break;
			case LEFT:
			printf("LEFT\n\r");
			break;
			case UP:
			printf("UP\n\r");
			break;
			case DOWN:
			printf("DOWN\n\r");
			break;
		}
		lastDirection = direction;
	}
}