#include "spi_lib.h"

void spi_setup(){
	/* Initialize as SPI master */
	
	/* set MISO pin as input */
	DDRB &= ~(1 << SPI_MISO);
	
	/* set SS, MOSI, SCK pins as outputs */ 
	DDRB |= ((1 << SPI_SS) | (1 << SPI_MOSI) | (1 << SPI_SCK));
	
	/* SPI Enable, master mode, frequency fosc/4 */
	SPCR |= ((1 << SPE) | (1 << MSTR));
}


uint8_t spi_transceive(uint8_t data){
	/* Wait for serial transfer complete flag */
	while(!(SPSR & (1<<SPIF)));
	SPDR = data;

	/* Wait for serial transfer complete flag */
	while(!(SPSR & (1<<SPIF)));

	/* Read SPI data register */	
	return SPDR;
}

uint8_t spi_read(){
	return spi_transceive(0xFF);
}
