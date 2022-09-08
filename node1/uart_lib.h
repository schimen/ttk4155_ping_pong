#ifndef UART_LIB_H_
#define UART_LIB_H_

#define BAUD 9600

#include "defines.h"
#include <stdio.h>
#include <avr/io.h>

int send_character(char data, FILE * file);

int receive_character(FILE * file);

void uart_setup(void);

#endif