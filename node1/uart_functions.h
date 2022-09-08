#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>

int send_character(char data, FILE * file);

int receive_character(FILE * file);

void usart_setup();


#endif