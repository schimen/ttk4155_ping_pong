#ifndef CAN_LIB_H_
#define CAN_LIB_H_

#include <avr/io.h>
#include <stdio.h>
#include "string.h"
#include "mcp2515_lib.h"

struct can_frame{
	uint16_t id; // Message id
	uint16_t ext_id; // Extended id
	uint8_t len;	// Data length
	uint8_t *data; 
};


void can_setup();

void can_write(struct can_frame *can_msg);

void can_receive(struct can_frame *can_msg);

void can_test();


#endif /* CAN_LIB_H_ */