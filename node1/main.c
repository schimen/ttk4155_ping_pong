#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include "util/delay.h"
#include "uart_lib.h"
#include "sram_lib.h"
#include "adc_lib.h"

char buffer[5];

int main(void)
{	
	uart_setup();
	sram_setup();
	//sram_test();
	//adc_setup();

	while(1){
		uint8_t val = ((uint8_t)0);
		itoa(val, buffer, 10);
		printf(buffer);
		printf("\r\n");
		_delay_ms(100);
		
	}
}