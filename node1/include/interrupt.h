#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>

/**
 * @brief  Interrupt API for node 1
 * @defgroup interrupt
 * @ingroup interrupt
 * @{
 * 
*/

/** Pin used for left touch button. INT0 on the ATMega162 */
#define LEFT_BUTTON PIND2
/** Pin used for right touch button. INT1 on the ATMega162 */
#define RIGHT_BUTTON PIND3
/** Pin used for MCP2515 interrupt. INT2 on the ATMega162 */
#define MCP_INT PINE0

/**
 * @brief Setup of interrupts on node 1. USB-board touchbuttons interrupt on rising edge.
 * CAN msg interrupt on falling edge.
 */
void interrupt_setup(void);

/** @} */

#endif /* INTERRUPT_H_ */