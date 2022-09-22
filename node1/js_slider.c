/*
 * js_slider.c
 *
 * Created: 19.09.2022 18:00:31
 *  Author: lineh
 */ 

#include "js_slider.h"
enum JS_position {DEFAULT, RIGHT, LEFT, UP, DOWN};

bool btn_down = false;


/* ADC-value to direction */
uint8_t get_JS_position(uint8_t x_val, uint8_t y_val){

	if(x_val < 150) { return LEFT; }
	else if(x_val > 180) { return RIGHT; }
	else if(y_val < 150) { return DOWN; }
	else if(y_val > 180) { return UP; }
	else {return DEFAULT;}
}

/* ADC-value to percentage */
uint8_t get_slider_position(uint8_t val){
	return (val*100)/255;
}

/* Converts joystick value from ADC to a direction and prints */
void JS_service(void){
	// Update ADC-values
	adc_read();
 	printf("x-val =%d\r\n", adc_result.x_val); // Raw JS x-value
 	printf("y-val=%d\r\n", adc_result.y_val); // Raw JS y-value
	
	/* Returns which direction the joystick is pointing */
	uint8_t JS_pos = get_JS_position(adc_result.x_val, adc_result.y_val);
	
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

/* Converts slider value from ADC to percentage and prints */
void silder_service(struct ADC_Values *adc_val)
{
	uint8_t l_slider = get_slider_position(adc_val->left_slider);
	uint8_t r_slider = get_slider_position(adc_val->right_slider);
	
// 	printf("Left slider =%d\r\n", l_slider);
// 	_delay_ms(100);
// 	printf("Right slider =%d\r\n", r_slider);
// 	_delay_ms(100);
}

void button_setup(void){
	//set as input
	DDRD &= ~(1 << LEFT_BUTTON); 
	DDRD &= ~(1 << RIGHT_BUTTON);
	btn_down = false; 
}

bool button_press(uint8_t button){
	/* the button is pressed when BUTTON bit is clear */
	if ((PIND & (1<<button)) && !btn_down) {
		btn_down = true;
		return false;
	}
	else if ((PIND & (1<<button)) && btn_down)
	{
		btn_down = false;
		return true;
	}
	else {return false;}
}