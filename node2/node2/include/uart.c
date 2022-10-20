/*
 * uart.c
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 
#include <stdint.h>

#include "sam.h"
#include "uart.h"

//Ringbuffer for receiving multiple characters
uart_ringbuffer rx_buffer;


/**
 * \brief Configure UART.
 *
 * \param void
 *
 * \retval void.
 */
void configure_uart(void)
{
	uint32_t ul_sr;

/*
Initialize UART ring buffer as empty
*/
rx_buffer.head=0;
rx_buffer.tail=0;

/*
Initialize UART communication
*/
	// Pin configuration
	// Disable interrupts on Uart receive (URXD) and transmit (UTXD) pins
	PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;

	// Disable the Parallel IO (PIO) of the URXD and UTXD pins so that the peripheral controller can use them
	PIOA->PIO_PDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;

	// Read current peripheral AB select register and set the UTXD and URXD pins to 0 (UART is connected as peripheral A)
	ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= ~(PIO_PA8A_URXD | PIO_PA9A_UTXD) & ul_sr;

	// Enable pull up resistor on URXD and UTXD pin
	PIOA->PIO_PUER = PIO_PA8A_URXD | PIO_PA9A_UTXD;

	// Uart configuration
	
	// Enable the peripheral UART controller in Power Management Controller (PMC)
	PMC->PMC_PCER0 = 1 << ID_UART;

	// Reset and disable receiver and transmitter
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	// Set the baudrate
	UART->UART_BRGR = 547; // MCK / 16 * x = BaudRate (write x into UART_BRGR)  

	// No parity bits
	UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;	

	// Disable PDC channel
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	// Configure interrupts on receive ready and errors
	UART->UART_IDR = 0xFFFFFFFF;
	UART->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME | UART_IER_PARE;

	// Enable UART interrupt in the Nested Vectored Interrupt Controller(NVIC)
	NVIC_EnableIRQ((IRQn_Type) ID_UART);

	// Enable UART receiver and transmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	
	

}

/**
 * \brief Get character from UART
 *
 * \param *c location of character 
 *
 * \retval Success(0) or failure(1)
 */
int uart_getchar(uint8_t *c)
{
	// Check if a character is available in the ringbuffer
	if(rx_buffer.head == rx_buffer.tail) { //Buffer is empty
		return 1;
	}

	// Read the head character from the ringbuffer
	*c = rx_buffer.data[rx_buffer.head];
	rx_buffer.head = (rx_buffer.head + 1) % UART_RINGBUFFER_SIZE;
	return 0;
}

/*
 * \brief Sends a character through the UART interface
 *
 * \param c Character to be sent
 *
 * \retval Success(0) or failure(1).
 */
int uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	if((UART->UART_SR & UART_SR_TXRDY) != UART_SR_TXRDY)
	return 1;

	// Send the character
	UART->UART_THR = c;
	while(!((UART->UART_SR) & UART_SR_TXEMPTY)); // Wait for the character to be sent, can implement ring buffer to remove the wait
	return 0;
}

void UART_Handler(void)
{
	uint32_t status = UART->UART_SR;
	
	//Reset UART at overflow error and frame error
	if(status & (UART_SR_OVRE | UART_SR_FRAME | UART_SR_PARE))
	{
		UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN | UART_CR_RSTSTA;
	}
	
	//Check if message is ready to be received
	if(status & UART_SR_RXRDY)
	{
		//Check if receive ring buffer is full and 
		if((rx_buffer.tail + 1) % UART_RINGBUFFER_SIZE == rx_buffer.head)
		{
			printf("ERR: UART RX buffer is full\n\r");
			rx_buffer.data[rx_buffer.tail] = UART->UART_RHR; //Throw away message
			return;
		}
		rx_buffer.data[rx_buffer.tail] = UART->UART_RHR;
		rx_buffer.tail = (rx_buffer.tail + 1) % UART_RINGBUFFER_SIZE;
		
	}
	
	
}