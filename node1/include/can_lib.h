#ifndef CAN_LIB_H_
#define CAN_LIB_H_

/**
 * @brief CAN library API for node 1
 * @defgroup can
 * @ingroup can
 * @{ 
*/

/** External oscillator frequency */
#define F_CPU 4915200

/** Baud rate prescaler. BRP = 3 yields a Time Quanta (TQ) = 500ns */
#define BRP		0x03
/** Propagation segment length = 2*/
#define PROPSEG 0x02
/** Phase segment 1 length = 7 */
#define PS1		0x07
/** Phase segment 2 length = 6 */
#define PS2		0x06

#include <avr/io.h>
#include <stdio.h>
#include "string.h"
#include "mcp2515_lib.h"
#include "util/delay.h"

/**
 * @brief CAN message struct
 * 
 */
struct can_frame{
	uint16_t id; // Message id
	uint8_t len;	// Data length
	uint8_t data[8]; 
};


/**
 * @brief Sets up CAN communication on the MCP2515
 * 
 */
void can_setup();


/**
 * @brief Transmits a message over CAN
 * 
 * @param can_msg Pointer to the message frame to be transmitted
 */
void can_write(struct can_frame *can_msg);


/**
 * @brief Get the number of new received messages
 * 
 * @param status_byte Byte representing rx status from can driver.
 * @return uint8_t Number of new messages ready to be received.
 */
uint8_t get_n_new_messages(uint8_t status_byte);


/**
 * @brief Receive can message
 * 
 * @param can_msg Pointer to can_frame struct where the new message will be 
 *   stored.
 * @return uint8_t Number of messages received
 */
uint8_t can_receive(struct can_frame *can_msg);


/**
 * @brief Test CAN communication sending messages with increasing IDs
 * 
 */
void can_test();

/** @} */

#endif /* CAN_LIB_H_ */