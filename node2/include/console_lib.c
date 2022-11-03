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
		//printf("DEFAULT\n\r");
		break;
		case RIGHT:
		if (currentServoPos > 0)
		{
			currentServoPos--;
			servo_set_pos(currentServoPos, 6);
			ms_delay(20);
		}
		//printf("RIGHT\n\r");
		break;
		case LEFT:
		if (currentServoPos < 180)
		{
			currentServoPos++;
			servo_set_pos(currentServoPos, 6);
			ms_delay(20);
		}
		//printf("LEFT\n\r");
		break;
		case UP:
		printf("UP\n\r");
		break;
		case DOWN:
		printf("DOWN\n\r");
		break;
	}
}
