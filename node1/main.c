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
#include "oled_lib.h"

int main(void)
{
	uart_setup();
	sram_setup();
	button_setup();

	oled_setup();
	
	/*Enable printf */
	fdevopen(send_character, receive_character);

	while (1)
	{
		// oled echo :)
		oled_putchar(uart_getchar());
	}
}