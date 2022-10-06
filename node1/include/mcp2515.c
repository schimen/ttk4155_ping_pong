#include "mcp2515.h"


uint8_t mcp2515_read(uint8_t address){
	/* send read instruction */
	spi_transceive(MCP_READ);
	/* send address */
	spi_transceive(address);
	
	/* receive data */
	uint8_t rx_data = spi_read(0xFF);
	
	return rx_data;
}

void mcp2515_write(uint8_t address, uint8_t data){
	spi_transceive(MCP_WRITE);
	spi_transceive(address);
	spi_transceive(data);
}

void mcp2515_request_to_send(){
	spi_transceive(MCP_RTS_TX0);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	spi_transceive(MCP_BITMOD);
	spi_transceive(address);
	spi_transceive(mask);
	spi_transceive(data);
}

void mcp2515_reset(){
	spi_transceive(MCP_RESET);
}

uint8_t mcp2515_read_status(){
	spi_transceive(MCP_READ_STATUS);
	uint8_t rx_data = spi_read();
	
	return rx_data;
}