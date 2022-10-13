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


uint8_t spi_transceiveByte(uint8_t data){
	SPDR = data;
	/* Wait for serial transfer complete flag */
	while(!(SPSR & (1 << SPIF)));
	/* Read SPI data register */	
	return SPDR;
}

int8_t spi_transceive(uint8_t *tx, uint8_t *rx, uint8_t txLen, uint8_t rxLen){
	int8_t status;
	DDRB &= ~(1 << SPI_SS);

	if (txLen && rxLen)
	{
		if (txLen < rxLen)
		{
			for (uint8_t i = 0; i < rxLen; i++)
			{
				if (i <= txLen) { SPDR = tx[i]; } 
				else { SPDR = 0xFF; }
				
				while(!(SPSR & (1 << SPIF)));
				rx[i] = SPDR;
			}		
		} else
		{
			for (uint8_t i = 0; i < txLen; i++)
			{
				SPDR = tx[i];
				while(!(SPSR & (1 << SPIF)));
				if (i <= rxLen){ rx[i] = SPDR; }
				else
				{
					char flushBuffer;
					flushBuffer = SPDR;	
				}
			}
		}
		status = sizeof(rx);
	} else if (txLen)
	{
		char flushBuffer;
		for (uint8_t i = 0; i < txLen; i++)
		{
			SPDR = tx[i];
			while(!(SPSR & (1 << SPIF)));
			flushBuffer = SPDR;
		}
	} else if(rxLen)
	{
		for (uint8_t i = 0; i < rxLen; i++)
		{
			SPDR = 0xFF;
			while(!(SPSR & (1 << SPIF)));
			rx[i] = SPDR;
		}
		status = sizeof(rx);
	} else
	{
		status = -1; //ERROR
	}
	
	DDRB |= (1 << SPI_SS);
	return status;
}

