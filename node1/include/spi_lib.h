#ifndef SPI_LIB_H_
#define SPI_LIB_H_

#include <stdio.h>
#include <avr/io.h>

#define SPI_SS		PB4 // Slave select
#define SPI_MOSI	PB5 // Master out, slave in
#define SPI_MISO	PB6 // Master in, slave out
#define SPI_SCK		PB7 // SPI clock

void spi_setup();

uint8_t spi_transceiveByte(uint8_t data);

int8_t spi_transceive(uint8_t *tx, uint8_t *rx, uint8_t txLen, uint8_t rxLen);

#endif