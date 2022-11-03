#ifndef CAN_LIB_H_
#define CAN_LIB_H_
#define F_CPU 4915200

#define BRP		0x03 // BRP = 3, TQ = 500ns
#define PROPSEG 0x02 // Propagation segment length 2
#define PS1		0x07 // Phase segment 1 length 7
#define PS2		0x06 // Phase segment 2 length 6

#include <avr/io.h>
#include <stdio.h>
#include "string.h"
#include "mcp2515_lib.h"
#include "util/delay.h"

struct can_frame{
	uint16_t id; // Message id
	uint8_t len;	// Data length
	uint8_t data[8]; 
};


void can_setup();

void can_write(struct can_frame *can_msg);

uint8_t can_receive(struct can_frame *can_msg);

void can_test();


#endif /* CAN_LIB_H_ */