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

/* External interrupts for buttons*/
ISR (INT0_vect)
{
	left_btn_pressed = true;
}

ISR (INT1_vect)
{
	right_btn_pressed = true;
}

int main(void)
{	
	uart_setup();
	sram_setup();
	button_setup();
	sei(); // Enable global interrupts
	calibrate_joystick();
	
	while(1)
	{
 		adc_read(); // Update ADC-values
 		JS_service();
 		silder_service();
		
		button_service();
		_delay_ms(500);
	}
}