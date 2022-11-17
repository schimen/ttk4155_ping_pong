#include "spi_lib.h"

void spi_setup(){
	/* set MISO pin as input */
	DDRB &= ~(1 << SPI_MISO);

	/* set SS, MOSI, SCK pins as outputs */ 
	DDRB |= ((1 << SPI_SS) | (1 << SPI_MOSI) | (1 << SPI_SCK));
	
	/* set SS-pin internal high */
	PORTB |= (1 << SPI_SS);
	
	/* SPI Enable, master mode, frequency fosc/4 */
	SPCR |= ((1 << SPE) | (1 << MSTR));
}


uint8_t spi_transceiveByte(uint8_t data){
	/* Pull slave select pin low */
	PORTB &= ~(1 << SPI_SS);
	
	/* Write transmit data to SPI data register */
	SPDR = data;
	/* Wait for serial transfer complete flag */
	while(!(SPSR & (1 << SPIF)));
	/* Read SPI data register */
	uint8_t result = SPDR;	
	PORTB |= (1 << SPI_SS);
	return result;
}

int8_t spi_transceive(uint8_t *tx, uint8_t *rx, uint8_t txLen, uint8_t rxLen){
	/* Pull slave select pin low */
	PORTB &= ~(1 << SPI_SS);

	/* Number of times shift register needs to be shifted */
	uint8_t loopLen = txLen + rxLen;

	for (uint8_t i = 0; i < loopLen; i++)
	{
		/* Write TX data to SPI data register if available */
		if (i < txLen && tx != NULL) {
			SPDR = tx[i];
		}
		/* If no TX data available, write dummy byte */
		else {
			SPDR = 0xFF;
		}
		/* Wait for serial data transfer complete flag */
		while(!(SPSR & (1 << SPIF)));

		/* Flush RX data while transmitting */
		if (i == 0 || i < txLen || rx == NULL) {
			char flushBuffer;
			flushBuffer = SPDR;
		}
		/* Save all RX data after transmission is done */
		else {
			rx[i-txLen] = SPDR;
		}
	}
	/* Pull slave select pin high */
	PORTB |= (1 << SPI_SS);
	return 0;
}

