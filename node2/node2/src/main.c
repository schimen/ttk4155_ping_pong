/*
 * node2.c
 *
 * Created: 20.10.2022 10:02:51
 * Author : joerg
 */ 


#include "sam.h"
#include "can_controller.h"

 #include "../include/uart.h"
 #include "../include/printf_stdarg.h"

 #define LED0_PIN PIO_PA19 
 #define LED1_PIN PIO_PA20

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable watchdogtimer
	configure_uart();
	printf("yeyyyy\r");

    while (1) 
    {
		PIOA->PIO_ODSR |= LED0_PIN;
		PIOA->PIO_ODSR |= LED1_PIN;	
    }
}

void LED_setup(void)
{
		// PORT enable IO
		PIOA->PIO_PER |= (LED0_PIN | LED1_PIN);
		// PORT pin set direction as output
		PIOA->PIO_OER |= (LED0_PIN | LED1_PIN);
		// PORT pin disable internal pullup
		PIOA->PIO_PUDR |= (LED0_PIN | LED1_PIN);
		// enable LED io to be written directly
		PIOA->PIO_OWER = (LED0_PIN | LED1_PIN);
}