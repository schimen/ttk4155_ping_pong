#include "uart_lib.h"

int send_character(char data, FILE * file)
{
	// Wait for available uart to send
	while (!(UCSR1A & (1 << UDRE1)));
	// Send data
	UDR1 = data;
	return 0;
}

int receive_character(FILE * file)
{
	// Wait for available data
	while (!(UCSR1A & (1 << RXC1)));
	return UDR1;
}

void uart_setup(void)
{
	unsigned long baud = BAUD;
	unsigned long clockspeed = F_CPU;
	unsigned long ubrr = clockspeed/16/baud-1;
	
	UBRR1H = (uint8_t)(ubrr >> 8); // Set baud rate
	UBRR1L = (uint8_t)ubrr;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8 bit data, 2 stop bit */
	UCSR1C = (1<<URSEL1)|(1<<USBS1)|(1<<UCSZ10)|(1<<UCSZ11);
	
	/*Enable printf */
	fdevopen (send_character, receive_character);
}