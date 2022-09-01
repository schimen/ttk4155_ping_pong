/*
 * node1.c
 *
 * Created: 01.09.2022 13:09:46
 * Author : joerg
 */ 

#include <avr/io.h>

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define BAUDRATE FOSC/16/BAUD-1

void usart_setup()
{
	UBRR1H = (uint8_t)(BAUDRATE >> 8); // Set baud rate
	UBRR1L = (uint8_t)BAUDRATE;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8 bit data, 2 stop bit */
	UCSR1C = (1<<URSEL1)|(1<<USBS1)|(3<<UCSZ01);
}


void send_character(uint8_t data)
{
	// Wait for available uart to send
	while (!(UCSR1A & (1 << UDRE1)));
	// Send data
	UDR1 = data;
}


uint8_t receive_character()
{
	// Wait for available data
	while (!(UCSR1A & (1 << RXC1)));
	return UDR1;
}


int main(void)
{
	/* Replace with your application code */
	usart_setup();
	send_character('f');
	uint8_t rx_data;
	while (1)
	{
		rx_data = receive_character;
		send_character(rx_data);
	}
}


