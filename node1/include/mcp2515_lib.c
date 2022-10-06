#include "mcp2515_lib.h"

uint8_t mcp2515_read(uint8_t address){
	PORTB &= ~(1 << SPI_SS);
	/* send read instruction */
	spi_transceive(MCP_READ);
	/* send address */
	spi_transceive(address);
	
	/* receive data */
	uint8_t rx_data = spi_read(0xFF);
	PORTB |= (1 << SPI_SS);
	
	return rx_data;
}

void mcp2515_write(uint8_t address, uint8_t data){
	PORTB &= ~(1 << SPI_SS);
	spi_transceive(MCP_WRITE);
	spi_transceive(address);
	spi_transceive(data);
	PORTB |= (1 << SPI_SS);
}

void mcp2515_request_to_send(){
	PORTB &= ~(1 << SPI_SS);
	spi_transceive(MCP_RTS_TX0);
	PORTB |= (1 << SPI_SS);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	PORTB &= ~(1 << SPI_SS);
	spi_transceive(MCP_BITMOD);
	spi_transceive(address);
	spi_transceive(mask);
	spi_transceive(data);
	PORTB |= (1 << SPI_SS);
}

void mcp2515_reset(){
	PORTB &= ~(1 << SPI_SS);
	spi_transceive(MCP_RESET);
	PORTB |= (1 << SPI_SS);
}

uint8_t mcp2515_read_status(){
	PORTB &= ~(1 << SPI_SS);
	spi_transceive(MCP_READ_STATUS);
	uint8_t rx_data = spi_read();
	spi_read();
	PORTB |= (1 << SPI_SS);
	return rx_data;
}

void mcp2515_setup_loopback()  {
	mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);
	mcp2515_write(MCP_CANINTF, MCP_TX2IF);
}