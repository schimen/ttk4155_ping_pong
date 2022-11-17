#ifndef UART_LIB_H_
#define UART_LIB_H_

/**
 * @brief UART library API for node 1
 * @defgroup uart
 * @ingroup uart
 * @{
 */

#include <stdio.h>
#include <avr/io.h>

/** UART baud rate */
#define BAUD 9600
/** External oscillator frequency */
#define F_CPU 4915200


/**
 * @brief Send character over UART
 * 
 * @param data char to send
 * @param file C stream
 * @return 0 when done transmitting
 */
int send_character(char data, FILE * file);


/**
 * @brief Receive character over UART
 * 
 * @return uint8_t Received byte
 */
uint8_t uart_getchar();


/**
 * @brief Setup stream for printf.
 * 
 * @param file C stream
 * @return int Received byte
 */
int receive_character(FILE * file);


/**
 * @brief Setup UART communication on node1
 * 
 */
void uart_setup(void);

/** @} */

#endif