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
	sram_test();
	while (1)
	{
		_delay_ms(500);
		printf("HELLO WORLD\r\n");
	}
}