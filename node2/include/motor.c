/*
 * motor.c
 *
 * Created: 08.11.2022 12:47:37
 *  Author: joerg
 */ 

#include "motor.h"

void motor_setup(){
	// DAC setup
    PMC->PMC_PCER1 |= PMC_PCER1_PID38; // Enable DAC clock in PMC
	// DACC in free running mode, half word mode, channel 1 selected, tag mode selected
	DACC->DACC_MR |= (DACC_MR_TRGEN_DIS | DACC_MR_WORD_HALF | DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_TAG_DIS);
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
	
    // Enable motor
    PIOD->PIO_ODSR |= MJ1_EN;
	
	encoder_calibrate();
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

void set_motor_speed(uint8_t speed){
    // Make sure speed is within range
    if (speed > 100) {
        speed = 100;
    }
    if (speed < 0) {
        speed = 0;
    }

	// Convert from percentage to 12 bit value
    uint32_t data = (speed*0xFFF)/100;
    
    // Write to lowest 12 bytes of DACC output register
	DACC->DACC_CDR = (data & 0xFFF);
}


int16_t encoder_read(){
    // Sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(MJ1_NOT_OE);
    // Set SEL low to output high byte
    PIOD->PIO_ODSR &= ~(MJ1_SEL);
    // Wait for output to settle
    ms_delay(1);
    // Read MJ2 to get high byte
	uint8_t encoder_high = (uint8_t) ((PIOC->PIO_PDSR & MJ2_PINS) >> 1);
    // Set SEL high to output low byte
    PIOD->PIO_ODSR |= MJ1_SEL;
    // Wait for output to settle
    ms_delay(1);
    // Read MJ2 to get low byte
    uint8_t encoder_low = (uint8_t) ((PIOC->PIO_PDSR & MJ2_PINS) >> 1);
    // Pull !OE high
    PIOD->PIO_ODSR |= MJ1_NOT_OE;

    // Add high and low byte together and return encoder value
    int16_t encoder = (int16_t) ((encoder_high << 8) | (encoder_low));
    return encoder;
}

uint8_t encoder_get_position(){
    // Read encoder
	int16_t raw_data = encoder_read();
	// Motor slipping might make encoder values negative
    if (raw_data < 0){
		raw_data = 0;
	}

    // Return position in range 0-100. 0 is rightmost, 100 is leftmost
	uint16_t position = (raw_data*100/encoder_max);
	return (uint8_t) position;
}

void encoder_reset(){
    // Pull !RST pin low
	PIOD->PIO_ODSR &= ~MJ1_NOT_RST;
	ms_delay(100);
    // Pull !RST pin high
	PIOD->PIO_ODSR |= MJ1_NOT_RST;
}

void encoder_calibrate(){
    // Drive all the way to the right
	set_motor_direction(MOTOR_RIGHT);
	set_motor_speed(60);
	ms_delay(10000);
    // Enable motor brake
	PIOD->PIO_ODSR &= ~MJ1_EN;
	set_motor_speed(0);
	// Reset encoder
    encoder_reset();
    // Enable motor
	PIOD->PIO_ODSR |= MJ1_EN;
    // Drive all the way to the left
	set_motor_direction(MOTOR_LEFT);
	set_motor_speed(60);
	ms_delay(10000);
    // Read maximum encoder value
	encoder_max = encoder_read();
    printf("Encoder calibration done! \n\r");
}
