#include "can_lib.h"


void can_setup() {
	// set the mcp in config mode
	mcp_write_byte(MCP_CANCTRL, MODE_CONFIG);
	
	// Write to CNF1, SJW = 1, BRP = 3
	mcp_write_byte(MCP_CNF1, (uint8_t) (SJW1 | BRP));

	// Write to CNF2, BTLmode enabled, Phase segment 1 = 7, Propagation segment = 2
	mcp_write_byte(MCP_CNF2, (uint8_t) (BTLMODE | (PS1 << 3) | PROPSEG));
	
	// Write to CNF3, SOF disabled, Wake-up filter disabled, PS2 = 6
	mcp_write_byte(MCP_CNF3, (uint8_t) (SOF_DISABLE | WAKFIL_DISABLE | PS2));
	
	// Filter off
	mcp_write_byte(MCP_RXB0CTRL, (0x03 << 5));
	 
	// Enable interrupt for RXB0 and RXB1 full
	mcp_write_byte(MCP_CANINTE, (MCP_RX0IF | MCP_RX1IF));
	
	// set the mcp in normal mode
	mcp_write_byte(MCP_CANCTRL, MODE_NORMAL);
}


void can_write(struct can_frame *can_msg) {
	uint8_t buffer[5 + can_msg->len];
	buffer[0] = (uint8_t) (can_msg->id >> 3);
	buffer[1] = (uint8_t) (0xE0 & (can_msg->id << 5));
	buffer[4] = (can_msg->len & 0x0F);
	memcpy(&buffer[5], can_msg->data, can_msg->len);
	_delay_us(700);
	mcp_load_txbuffer(TXBUF0_START_ID, buffer, sizeof(buffer));
	mcp_request_to_send();
}


uint8_t get_n_new_messages(uint8_t status_byte) {
	// Get the two top bits (corresponding to buffer numbers in status byte)
	uint8_t buffer_number = (status_byte >> 6) & 0x03;
	// Return number of new messages based on buffer_number
	if (buffer_number > 2) {
		return 2;
	}
	else if (buffer_number > 0) {
		return 1;
	}
	else {
		return 0;
	}
}


uint8_t can_receive(struct can_frame *can_msg)
{	
	uint8_t rx_status = mcp_read_rx_status();
	if (get_n_new_messages(rx_status) == 0) {
		// No new messages, return 0
		return 0;
	}
	else {
		uint8_t buffer[5];
		// Set buffer number to read_mode for specifying buffer
		uint8_t buffer_number = (rx_status >> 7) & 0x01;
		uint8_t read_mode = (buffer_number << 1) | RXBUF0_START_ID;
		mcp_read_rxbuffer(read_mode, buffer, 5);
		
		can_msg->id = (buffer[0] << 3) | (buffer[1] >> 5);
		can_msg->len = buffer[4] & 0x0F;

		// Set buffer number to new read mode when we want to start reading data
		read_mode = (buffer_number << 1) | RXBUF0_START_DATA;
		mcp_read_rxbuffer(read_mode, can_msg->data, can_msg->len);
		// Received 1 message, return 1
		return 1;
	}
}


void can_test()
{
	struct can_frame can_tx_msg = {
		.data={ 0, 0, 0, 0, 0, 0, 0},
		.id=123,
		.len=7
	};
	
	for (uint8_t i = 0; i < 32; i++) {
		printf("Test %d:\n", i);
		can_tx_msg.id = i;
		for (uint8_t j = 0; j < can_tx_msg.len; j++) {
			can_tx_msg.data[j] = i+j;
		}
		
		printf("Sending message (%d length): ", can_tx_msg.len);
		
		for (uint8_t i = 0; i < can_tx_msg.len; i++) {
			printf("%d, ", can_tx_msg.data[i]);
		}
		printf(" from id %d\n", can_tx_msg.id);
		can_write(&can_tx_msg);
		
		_delay_ms(100);
	}
}
