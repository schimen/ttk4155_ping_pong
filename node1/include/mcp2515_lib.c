#include "mcp2515_lib.h"

uint8_t mcp_read_byte(uint8_t address) {
	/* Transmit read instruction and address */
	uint8_t tx[2] = {MCP_READ, address};
	/* Receive one byte */ 
	uint8_t rx[1];
	spi_transceive(tx, rx, 2, 1);
	
	return rx[0];
}


void mcp_read_rxbuffer(uint8_t read_mode, uint8_t * buffer, uint8_t len) {
	/* Read MCP RX buffer defined by read_mode */
	uint8_t tx[] = {MCP_READ_RX0 | (read_mode << 1)};
	spi_transceive(&tx, buffer, 1, len);
}


void mcp_write_byte(uint8_t address, uint8_t data) {
	/* Write */
	uint8_t tx[3] = {MCP_WRITE, address, data};
	spi_transceive(&tx, NULL, 3, 0);
}


void mcp_load_txbuffer(uint8_t write_mode, uint8_t * data, uint8_t len) {
	/* Make new buffer array with load tx buffer command as first element */
	uint8_t tx[len+1];
	tx[0] = MCP_LOAD_TX0 | write_mode;
	memcpy(&tx[1], data, len);
	
	spi_transceive(tx, NULL, len+1, 0);
}

void mcp_request_to_send() {
	spi_transceiveByte(MCP_RTS_TX0);
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	uint8_t tx[4] = {MCP_BITMOD, address, mask, data};
	spi_transceive(tx, NULL, 4, 0);
}

void mcp_reset() {
	spi_transceiveByte(MCP_RESET);
}

uint8_t mcp_read_status() {
	return spi_transceiveByte(MCP_READ_STATUS);
}

uint8_t mcp_read_rx_status() {
	return spi_transceiveByte(MCP_RX_STATUS);
}

void mcp_setup_loopback() {
	/* */
	mcp_write_byte(MCP_CANCTRL, MODE_LOOPBACK);
	mcp_write_byte(MCP_CANINTF, MCP_TX2IF);
}