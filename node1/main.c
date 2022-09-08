#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include "util/delay.h"
#include "uart_functions.h"

int main(void)
{	
	usart_setup();

	while (1)
	{
		_delay_ms(500);
		printf("HELLO WORLD\r\n");
	}
}