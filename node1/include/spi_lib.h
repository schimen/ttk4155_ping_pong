#ifndef SPI_LIB_H_
#define SPI_LIB_H_

#include <avr/io.h>

#define SPI_SS		PB4 // Slave select
#define SPI_MOSI	PB5 // Master out, slave in
#define SPI_MISO	PB6 // Master in, slave out
#define SPI_SCK		PB7 // SPI clock

void spi_setup();

uint8_t spi_transceive(uint8_t data);

uint8_t spi_read();

#endif