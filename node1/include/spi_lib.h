#ifndef SPI_LIB_H_
#define SPI_LIB_H_

/**
 * @brief SPI library API for node 1 
 * @defgroup spi
 * @ingroup spi
 * @{
 */

#include <stdio.h>
#include <avr/io.h>

/** Slave select pin */
#define SPI_SS		PB4
/** Master out, slave in pin */
#define SPI_MOSI	PB5
/** Master in, slave out pin*/
#define SPI_MISO	PB6
/** SPI clock pin */
#define SPI_SCK		PB7


/**
 * @brief Initialize SPI on the ATMega162
 * 
 */
void spi_setup();


/**
 * @brief Transmit and receive one byte over SPI
 * 
 * @param data Byte to transmit
 * @return Received byte
 */
uint8_t spi_transceiveByte(uint8_t data);


/**
 * @brief Transmit and receive bytes over SPI
 * 
 * @param tx Pointer to transmit buffer
 * @param rx Pointer to receive buffer
 * @param txLen Number of bytes to transmit
 * @param rxLen Number of bytes to receive
 * @return 0 when transceive operation is done
 */
int8_t spi_transceive(uint8_t *tx, uint8_t *rx, uint8_t txLen, uint8_t rxLen);

/** @} */

#endif