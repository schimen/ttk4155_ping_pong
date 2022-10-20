/*
 * node2.c
 *
 * Created: 20.10.2022 10:02:51
 * Author : joerg
 */ 


#include "sam.h"
#include "can_controller.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	uint32_t brp = (CAN_BR_BRP(41) | CAN_BR_SJW(1) | CAN_BR_PROPAG(2) | CAN_BR_PHASE1(7) | CAN_BR_PHASE2(6));
	can_init_def_tx_rx_mb(brp);
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}