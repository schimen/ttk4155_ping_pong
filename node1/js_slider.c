/*
 * js_slider.c
 *
 * Created: 19.09.2022 18:00:31
 *  Author: lineh
 */ 

#include "js_slider.h"
#define TRIGGER_POINT 50 //percent

enum JS_position {DEFAULT, RIGHT, LEFT, UP, DOWN};

/* Averages 10 adc-measurements to set the default adc-values for the joystick in default position */
void calibrate_joystick(void)
{
	uint16_t x_sum_adc = 0;
	uint16_t y_sum_adc = 0;
	
	for (int x = 0; x < 10; x++)
	{
		adc_read();
		x_sum_adc += joystick.x_adc;
		y_sum_adc += joystick.y_adc;
	}
	
	joystick.x_default = (x_sum_adc/10);
	joystick.y_default = (y_sum_adc/10);
}

/* Converts adc-value of the joystick to a percentage*/
int8_t get_JS_position(uint8_t adc_val, uint8_t default_val)
{
	if (adc_val < default_val) //LEFT or DOWN, Values: 0% to -100%
	{
		return -(100 - ((adc_val*100) / default_val));
	}
	else if (adc_val > default_val) //RIGHT or UP, Values: 0% to 100%
	{
		return ((adc_val-default_val)*100) / (255-default_val);
	}
	else // DEFAULT
	{
		return 0;
	}	
}

/* Percent-value of the joystick to a direction */
uint8_t get_JS_direction(int8_t x_val, int8_t y_val)
{
	if(x_val < -TRIGGER_POINT) { return LEFT; }
	else if(x_val > TRIGGER_POINT) { return RIGHT; }
	else if(y_val < -TRIGGER_POINT) { return DOWN; }
	else if(y_val > TRIGGER_POINT) { return UP; }
	else {return DEFAULT;}
}

/* Silder ADC-value to percentage */
uint8_t get_slider_position(uint8_t val)
{
	return (val*100)/255;
}

/* Setup for USB-board touch-buttons with interrupt on rising edge */
void button_setup(void)
{
	// Set button pins as input
	DDRD &= ~((1 << LEFT_BUTTON) | (1 << RIGHT_BUTTON)); 
	
	// INT0 and INT1 triggers on rising edge
 	MCUCR |= 0x0F; 
 	
	 //Enable INT0 and INT1
	GICR = (1 << INT0) | (1 << INT1); 
	
	//Flags to detect button press 
	left_btn_pressed = false;
	right_btn_pressed = false;
}


/*------------- PLACEHOLDER FUNCTIONS -------------*/
/* Placeholder function for joystick: Converts joystick value from ADC to a direction and prints it */
void JS_service(void)
{
	/*Converts adc-value to position in percent*/
	joystick.x_pos = get_JS_position(joystick.x_adc, joystick.x_default);
	joystick.y_pos = get_JS_position(joystick.y_adc, joystick.y_default);
	printf("Joystick X:%d Y:%d \r\n", joystick.x_pos, joystick.y_pos);
	
	/* Returns which direction the joystick is pointing */
	uint8_t JS_pos = get_JS_direction(joystick.x_pos, joystick.y_pos);
	
	/* Prints which direction the joystick is pointing */
	switch (JS_pos)
	{
		case DEFAULT:
		printf("DEFAULT\r\n");
		break;
		case RIGHT:
		printf("RIGHT\r\n");
		break;
		case LEFT:
		printf("LEFT\r\n");
		break;
		case UP:
		printf("UP\r\n");
		break;
		case DOWN:
		printf("DOWN\r\n");
		break;
	}
}

/* Placeholder function for sliders: Converts slider value from ADC to percentage and prints it */
void silder_service(void)
{
	/* Converts adc-value to position in percent */
	slider.left_pos = get_slider_position(slider.left_adc);
	slider.right_pos = get_slider_position(slider.right_adc);
	printf("Slider L:%d R:%d \r\n", slider.left_pos, slider.right_pos);
}

/* Placeholder function for what happens when one of the buttons is pushed */
void button_service(void){
	if (left_btn_pressed)
	{
		printf("LEFT BUTTON PRESS\r\n");
		left_btn_pressed = false;
	}
	if (right_btn_pressed)
	{
		printf("RIGHT BUTTON PRESS\r\n");
		right_btn_pressed = false;
	}
}