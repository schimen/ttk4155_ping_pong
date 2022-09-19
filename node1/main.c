#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
#include "uart_lib.h"
#include "sram_lib.h"
#include "adc_lib.h"

ISR(TIMER1_COMPA_vect)
{
	// Toggle PD5 to create clock signal
	PORTD ^= (1 << PD5);
}

void interrupt_setup()
{
    // Set PD5 (clock pin) to output
	DDRD |= (1 << PD5);
	// CTC mode with prescaler of 1 at timer 1 (for clock signal)
	TCCR1B = (1 << CS10) | (1 << WGM12);
  	TCCR1A = (1 << COM1A0);
	// Output compare value for clock signal at timer 1
	OCR1A = 1;
	// Clear output compare interrupt flag at timer 1
	TIFR = (1 << OCF1A);
	// Enable output compare interrupt at timer 1 (clock)
	TIMSK = (1 << OCIE1A);
	// Enable interrupts
	sei();
}

int main(void)
{	
	interrupt_setup();
	uart_setup();
	sram_setup();
	sram_test();

	while(1){
		sram_write(((uint16_t) 0x1800), (uint8_t) 0);
		_delay_ms(100);
	}
}