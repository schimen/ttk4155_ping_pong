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
	// DACC in free running mode, half word mode, channel 1 selected, tag mode selected
	DACC->DACC_MR |= (DACC_MR_TRGEN_DIS | DACC_MR_WORD_HALF | DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_TAG_DIS);
    DACC->DACC_CHER |= DACC_CHER_CH1; // DAC1 enable
	
	if (~(DACC->DACC_CHSR & DACC_CHSR_CH1)){
		printf("DAC channel 1 not enabled\n\r");
	}
	

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
	uint32_t data = (speed*0xFFF)/100;
	
	if (data < MOTOR_TRESHOLD && data != 0){data = MOTOR_TRESHOLD;}
	DACC->DACC_CDR = (data & 0xFFF);
	//printf("setting motor speed: %d\n\r", (data & 0xFFF));
}


int16_t encoder_read(){
    PIOD->PIO_ODSR &= ~(MJ1_NOT_OE);										// 1: Set !OE low, to sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(MJ1_SEL);											// 2: Set SEL low to output high byte
    ms_delay(1);															// 3: Wait approx. 20 microseconds for output to settle
	uint8_t encoder_high = (uint8_t) ((PIOC->PIO_PDSR & MJ2_PINS) >> 1);	// 4: Read MJ2 to get high byte
    PIOD->PIO_ODSR |= MJ1_SEL;												// 5: Set SEL high to output low byte
    ms_delay(1);															// 6: Wait approx. 20 microseconds
    uint8_t encoder_low = (uint8_t) ((PIOC->PIO_PDSR & MJ2_PINS) >> 1);		// 7: Read MJ2 to get low byte
    PIOD->PIO_ODSR |= MJ1_NOT_OE;											// 8: Set !OE to high

    int16_t encoder = (int16_t) ((encoder_high << 8) | (encoder_low));
	
    return encoder;
}

uint8_t encoder_get_position(){
	int16_t raw_data = encoder_read();
	if (raw_data < 0){
		raw_data = 0;
	}
	uint16_t position = (raw_data*100/encoder_max);
	return (uint8_t) position;
}

void encoder_reset(){
	PIOD->PIO_ODSR &= ~MJ1_NOT_RST;
	ms_delay(100);
	PIOD->PIO_ODSR |= MJ1_NOT_RST;
}

void encoder_calibrate(){
	uint8_t current_pos = encoder_get_position();
	set_motor_direction(MOTOR_RIGHT);
	set_motor_speed(60);
	ms_delay(10000);
	//Keep driving as long as the position changes
// 	while (~(current_pos == encoder_get_position())){
// 		printf("calibrating..\n\r");
// 		current_pos = encoder_get_position();
// 		ms_delay(10);
// 	}
	PIOD->PIO_ODSR &= ~MJ1_EN;
	set_motor_speed(0);
	encoder_reset();
	PIOD->PIO_ODSR |= MJ1_EN;
	printf("calibration done..%d \n\r", encoder_get_position());
	set_motor_direction(MOTOR_LEFT);
	set_motor_speed(60);
	ms_delay(10000);
	encoder_max = encoder_read();
	printf("encoder max: %d \n\r", encoder_max);
	
}
