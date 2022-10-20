/*
 * can_controller.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_controller.h"

#include "sam.h"

#include "../uart_and_printf/printf-stdarg.h"


/**
 * \brief Initialize can bus with predefined number of rx and tx mailboxes, 
 * CAN0->CAN_MB[0] is used for transmitting
 * CAN0->CAN_MB[1,2] is used for receiving
 *			
 * \param can_br Value to be set in CAN0->CAN_BR register to match can bus bit timing
 *
 * \retval Success(0) or failure(1)
 */
uint8_t can_init_def_tx_rx_mb(uint32_t can_br)
{
	return can_init(can_br, 1, 2);
}

/**
 * \brief Initialize can bus
 *
 * \param can_br Value to be set in CAN0->CAN_BR register to match can bus bit timing
 *
 * \param num_tx_mb Number of transmit mailboxes, 	tx mb indexes: [0		, num_tx_mb-1]
 *
 * \param num_rx_mb Number of receive mailboxes, 	rx mb indexes: [num_tx_mb, num_rx_mb-1]
 *
 * \retval Success(0) or failure(1)
 */


uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb)
{
	
	//Make sure num_rx_mb and num_tx_mb is valid
	if(num_rx_mb > 8 | num_tx_mb > 8 | num_rx_mb + num_tx_mb > 8)
	{
		return 1; //Too many mailboxes is configured
	}


	uint32_t ul_status; 
	
	//Disable can
	CAN0->CAN_MR &= ~CAN_MR_CANEN; 
	//Clear status register on read
	ul_status = CAN0->CAN_SR; 
	
	
	// Disable interrupts on CANH and CANL pins
	PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	//Select CAN0 RX and TX in PIOA
	uint32_t ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR = ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0) & ul_sr;
	
	// Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral controller can use them
	PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;
	
	// Enable pull up on CANH and CANL pin
	PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);
	
	
	//Enable Clock for CAN0 in PMC
	PMC->PMC_PCR = PMC_PCR_EN | (0 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_CAN0 << PMC_PCR_PID_Pos); // DIV = 1(can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
	PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);
	
	//Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match on all nodes!
	CAN0->CAN_BR = can_br; 
	

	/****** Start of mailbox configuration ******/

	uint32_t can_ier = 0;

	/* Configure receive mailboxes */
	for (int n = num_tx_mb; n <= num_rx_mb + num_tx_mb; n++)  //Simply one mailbox setup for all messages. You might want to apply filter for them.
	{
		CAN0->CAN_MB[n].CAN_MAM = 0; //Accept all messages
		CAN0->CAN_MB[n].CAN_MID = CAN_MID_MIDE;
		CAN0->CAN_MB[n].CAN_MMR = (CAN_MMR_MOT_MB_RX);
		CAN0->CAN_MB[n].CAN_MCR |= CAN_MCR_MTCR;

		can_ier |= 1 << n; //Enable interrupt on rx mailbox
	}
	
	/*Configure transmit mailboxes */
	for (int n = 0; n < num_tx_mb; n++)
	{
		CAN0->CAN_MB[n].CAN_MID = CAN_MID_MIDE;
		CAN0->CAN_MB[n].CAN_MMR = (CAN_MMR_MOT_MB_TX);
	}
	
	/****** End of mailbox configuraion ******/

	//Enable interrupt on receive mailboxes
	CAN0->CAN_IER = can_ier;

	//Enable interrupt in NVIC 
	NVIC_EnableIRQ(ID_CAN0);

	//enable CAN
	CAN0->CAN_MR |= CAN_MR_CANEN;

	return 0;
}

/**
 * \brief Send can message from mailbox
 *
 * \param can_msg message to be sent
 *
 * \param tx_mb_id ID of transmit mailbox to be used
 *
 * \retval Success(0) or failure(1)
 */
uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t tx_mb_id)
{
	//Check that mailbox is ready
	if(CAN0->CAN_MB[tx_mb_id].CAN_MSR & CAN_MSR_MRDY)
	{
		//Set message ID and use CAN 2.0B protocol
		CAN0->CAN_MB[tx_mb_id].CAN_MID = CAN_MID_MIDvA(can_msg->id) | CAN_MID_MIDE ;
		
		//Make sure message is not to long
		if(can_msg->data_length > 7){
			can_msg->data_length = 7;
			//Message is to long, sending only the first 8 bytes
		}
		//Put message in can data registers
		CAN0->CAN_MB[tx_mb_id].CAN_MDL = can_msg->data[3] << 24 | can_msg->data[2] << 16 | can_msg->data[1] << 8 | can_msg->data[0];
		CAN0->CAN_MB[tx_mb_id].CAN_MDH = can_msg->data[7] << 24 | can_msg->data[6] << 16 | can_msg->data[5] << 8 | can_msg->data[4];
		
		//Set message length and mailbox ready to send
		CAN0->CAN_MB[tx_mb_id].CAN_MCR = (can_msg->data_length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
		return 0;
	}
	
	else //Mailbox busy
	{
		return 1;
	}
	
}

/**
 * \brief Read can message from mailbox
 *
 * \param can_msg struct instance to save received data
 *
 * \param rx_mb_id ID of receive mailbox to be used
 *
 * \retval Success(0) or failure(1)
 */
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t rx_mb_id)
{
	//Check that mailbox is ready
	if(CAN0->CAN_MB[rx_mb_id].CAN_MSR & CAN_MSR_MRDY)
	{
		//Get data from CAN mailbox
		uint32_t data_low = CAN0->CAN_MB[rx_mb_id].CAN_MDL;
		uint32_t data_high = CAN0->CAN_MB[rx_mb_id].CAN_MDH;
		
		//Get message ID
		can_msg->id = (uint16_t)((CAN0->CAN_MB[rx_mb_id].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
		
		//Get data length
		can_msg->data_length = (uint8_t)((CAN0->CAN_MB[rx_mb_id].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
		
		//Put data in CAN_MESSAGE object
		for(int i = 0; i < can_msg->data_length;i++)
		{
			if(i < 4)
			{
				can_msg->data[i] = (char)(data_low & 0xff);
				data_low = data_low >> 8;
			}
			else
			{
				can_msg->data[i] = (uint8_t)(data_high & 0xff);
				data_high = data_high >> 8;
			}
		}
		
		//Reset for new receive
		CAN0->CAN_MB[rx_mb_id].CAN_MMR = CAN_MMR_MOT_MB_RX;
		CAN0->CAN_MB[rx_mb_id].CAN_MCR |= CAN_MCR_MTCR;
		return 0;
	}
	else //Mailbox busy
	{
		return 1;
	}
}

