#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include "util/delay.h"
#include "uart_lib.h"
#include "sram_lib.h"
	
int main(void)
{	
	uart_setup();
	sram_setup();
	//sram_test();

	while(1){
		sram_write(((uint16_t) 0x1800), (uint8_t) 0);
		_delay_ms(100);
	}
}