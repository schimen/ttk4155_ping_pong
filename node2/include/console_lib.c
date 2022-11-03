/*
 * joystick.c
 *
 * Created: 27.10.2022 14:58:34
 *  Author: lineh
 */ 
#include "console_lib.h"

void JS_Handler(uint8_t direction)
{	
	switch (direction)
	{
		case DEFAULT:
		printf("DEFAULT\r");
		break;
		case RIGHT:
		printf("RIGHT\r");
		break;
		case LEFT:
		printf("LEFT\r");
		break;
		case UP:
		printf("UP\r");
		break;
		case DOWN:
		printf("DOWN\r");
		break;
	}
}