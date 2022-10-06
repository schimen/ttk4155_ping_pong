#ifndef UART_LIB_H_
#define UART_LIB_H_

#define BAUD 9600
#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>

int send_character(char data, FILE * file);

uint8_t uart_getchar();

int receive_character(FILE * file);

void uart_setup(void);

#endif