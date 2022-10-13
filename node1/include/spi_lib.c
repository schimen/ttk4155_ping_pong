#include "spi_lib.h"

void spi_setup(){
	/* Initialize as SPI master */
	
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
	PORTB &= ~(1 << SPI_SS);
	
	SPDR = data;
	/* Wait for serial transfer complete flag */
	while(!(SPSR & (1 << SPIF)));
	/* Read SPI data register */
	uint8_t result = SPDR;	
	PORTB |= (1 << SPI_SS);
	return result;
}

int8_t spi_transceive(uint8_t *tx, uint8_t *rx, uint8_t txLen, uint8_t rxLen){
	PORTB &= ~(1 << SPI_SS);
	uint8_t loopLen;
	if (txLen > rxLen) {
		loopLen = txLen;
	}
	else {
		loopLen = rxLen + 1;
	}
	for (uint8_t i = 0; i < loopLen; i++)
	{
		if (i < txLen && tx != NULL) {
			SPDR = tx[i];
		}
		else {
			SPDR = 0xFF;
		}
		while(!(SPSR & (1 << SPIF)));
		if (i == 0 || i > rxLen || rx == NULL) {
			char flushBuffer;
			flushBuffer = SPDR;
		}
		else {
			rx[i-1] = SPDR;
		}
	}
	PORTB |= (1 << SPI_SS);
	return 0;
}

