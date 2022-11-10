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
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;
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
	PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    PIOC->PIO_PER |= MJ2_PINS;
    // Set pins as input
    PIOC->PIO_ODR |= MJ2_PINS;
    // Disable internal pull up
    PIOC->PIO_PUDR |= MJ2_PINS;
	
	
	// Pull encoder output enable and reset pin high
	PIOD->PIO_ODSR |= (MJ1_NOT_OE | MJ1_NOT_RST);
}


void set_motor_direction(uint8_t direction){
    switch (direction)
    {
        case MOTOR_RIGHT:
            PIOD->PIO_ODSR |= MJ1_DIR;
            break;
        
        case MOTOR_LEFT:
            PIOD->PIO_ODSR &= ~(MJ1_DIR);
            break;
    }
}


int16_t encoder_read(){
    PIOD->PIO_ODSR &= ~(MJ1_NOT_OE);										// 1: Set !OE low, to sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(MJ1_SEL);											// 2: Set SEL low to output high byte
    ms_delay(10);															// 3: Wait approx. 20 microseconds for output to settle
    uint8_t encoder_high = (uint8_t) ((PIOC->PIO_PDSR & MJ2_PINS) >> 1);	// 4: Read MJ2 to get high byte
    PIOD->PIO_ODSR |= MJ1_SEL;												// 5: Set SEL high to output low byte
    ms_delay(10);															// 6: Wait approx. 20 microseconds
    uint8_t encoder_low = (uint8_t) ((PIOC->PIO_PDSR & MJ2_PINS) >> 1);		// 7: Read MJ2 to get low byte
    PIOD->PIO_ODSR |= MJ1_NOT_OE;											// 8: Set !OE to high

    int16_t encoder = (int16_t) ((encoder_high << 8) | (encoder_low));
    return encoder;
}

void encoder_reset(){
	PIOD->PIO_ODSR &= ~MJ1_NOT_RST;
	ms_delay(100);
	PIOD->PIO_ODSR |= MJ1_NOT_RST;
}

void encoder_calibrate(){
	int16_t current_pos = encoder_read();
	set_motor_direction(MOTOR_RIGHT);
	// TODO: set_motor_speed();
	// Keep driving as long as the position changes
	while (~(current_pos == encoder_read())){
		current_pos = encoder_read();
		ms_delay(20);
	}
	// set_motor_speed(0);
	encoder_reset();
}
