#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
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
	
	while(1){
		JS_service();
		silder_service(&adc_result);
		_delay_ms(500);
	}
}