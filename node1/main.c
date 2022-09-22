#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "util/delay.h"
#include "uart_lib.h"
#include "sram_lib.h"
#include "adc_lib.h"
#include "js_slider.h"

char buffer[5];

int main(void)
{	
	uart_setup();
	sram_setup();
	//sram_test();
	button_setup();
	
	while(1){
		JS_service();
		silder_service(&adc_result);
		
		if (button_press(LEFT_BUTTON))
		{
			printf("LEFT BUTTON PRESS\r\n");
		}
		if (button_press(RIGHT_BUTTON))
		{
			printf("RIGHT BUTTON PRESS\r\n");
		}
		_delay_ms(200);
	}
}