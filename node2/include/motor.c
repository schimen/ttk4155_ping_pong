/*
 * motor.c
 *
 * Created: 08.11.2022 12:47:37
 *  Author: joerg
 */ 

#include "motor.h"

void motor_setup(){
    // Setup of the pins that interfaces with the motorbox

	// DAC setup
    PMC->PMC_PCER1 |= PMC_PCER1_PID38; // Enable DAC clock in PMC
    DACC->DACC_CHER |= DACC_CHER_CH1; // DAC1 enable


    // MJ1 SETUP
    // Enable MJ1 pins
    PIOD->PIO_PER |= (MJ1_NOT_OE | MJ1_NOT_RST | MJ1_SEL | MJ1_EN | MJ1_DIR);
    // Set pins as output
    PIOD->PIO_OER |= (MJ1_NOT_OE | MJ1_NOT_RST | MJ1_SEL | MJ1_EN | MJ1_DIR);
    // Enable internal pull up for encoder enable and reset pins
    PIOD->PIO_PUER |= (MJ1_NOT_OE | MJ1_NOT_RST);
    // Disable internal pull up for select, enable and direction pins
    PIOD->PIO_PUDR |= (MJ1_SEL | MJ1_EN | MJ1_DIR);
    // Enable pins to be written directly using ODSR
    PIOD->PIO_OWER |= (MJ1_NOT_OE | MJ1_NOT_RST | MJ1_SEL | MJ1_EN | MJ1_DIR);

    // MJ2 SETUP
    // Enable MJ2 pins
    PIOC->PIO_PER |= MJ2_PINS;
    // Set pins as input
    PIOC->PIO_ODR |= MJ2_PINS;
    // Disable internal pull up
    PIOC->PIO_PUDR |= MJ2_PINS;
}


void set_motor_direction(uint8_t direction){
    switch (direction)
    {
        case RIGHT:
            PIOD->PIO_ODSR |= MJ1_DIR;
            break;
        
        case LEFT:
            PIOD->PIO_ODSR &= ~(MJ1_DIR);
            break;
    }
}


uint16_t encoder_read(){
    PIOD->PIO_ODSR &= ~(MJ1_NOT_OE);                    // 1: Set !OE low, to sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(MJ1_SEL);                       // 2: Set SEL low to output high byte
    //_delay_us(20);                                      // 3: Wait approx. 20 microseconds for output to settle
    uint8_t encoder_high = (PIOC->PIO_PDSR & MJ2_PINS); // 4: Read MJ2 to get high byte
    PIOD->PIO_ODSR |= MJ1_SEL;                          // 5: Set SEL high to output low byte
    //_delay_us(20);                                      // 6: Wait approx. 20 microseconds
    uint8_t encoder_low = (PIOC->PIO_PDSR & MJ2_PINS);  // 7: Read MJ2 to get low byte
    PIOD->PIO_ODSR |= MJ1_NOT_OE;                       // 8: Set !OE to high

    uint16_t encoder = (uint16_t) ((encoder_high << 8) | (encoder_low));
    return encoder;
}
