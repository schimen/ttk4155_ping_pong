#include "can_lib.h"

void can_setup() {
	/* Set up mcp in loopback mode */ 
	mcp_setup_loopback();
	
}


void can_write(struct can_frame *can_msg) {
	uint8_t buffer[5+ can_msg->len];
	buffer[0] = (uint8_t) can_msg->id >> 8;
	buffer[1] = (uint8_t) (0xFF & can_msg->id);
	buffer[2] = (uint8_t) can_msg->ext_id >> 8;
	buffer[3] = (uint8_t) (0xFF & can_msg->ext_id);
	buffer[4] = can_msg->len;
	memcpy(&buffer[5], can_msg->data, can_msg->len);

	mcp_load_txbuffer(TXBUF0_START_ID, buffer);
	
	mcp_request_to_send();
}


void can_receive(struct can_frame *can_msg) {
	uint8_t rx_status = mcp_read_rx_status();
	
	if(rx_status == 1 || rx_status == 3){
		uint8_t buffer[5];
		mcp_read_rxbuffer(RXBUF0_START_ID, buffer, 5);
		
		can_msg->id = (buffer[0] << 8) | buffer[1];
		can_msg->ext_id = (buffer[2] << 8) | buffer[3];
		can_msg->len = buffer[4];
		
		mcp_read_rxbuffer(RXBUF0_START_DATA, can_msg->data, can_msg->len);
	}
}

void test_can(){
	struct can_frame can_tx_msg;
	can_tx_msg.id = 123;
	can_tx_msg.len	= 3;
	can_tx_msg.data = {4,2,0};
	
	struct can_frame can_rx_msg;
	
	can_write(&can_tx_msg);
	
	can_receive(&can_rx_msg);
	
	printf("Sending: %d%d%d", can_tx_msg.data[0], can_tx_msg.data[1], can_tx_msg.data[2]);
	printf("Receiving: %d%d%d", can_rx_msg.data[0], can_rx_msg.data[1], can_rx_msg.data[2]);
	
	
}
