/*
 * motor.c
 *
 * Created: 08.11.2022 12:47:37
 *  Author: joerg
 */ 

#include "motor.h"


void motor_box_setup(){
    // MJ1 SETUP
    // Enable MJ1 pins
    PIOD->PIO_PER |= (MJ1_NOT_OE | MJ1_NOT_RST | MJ1_SEL | MJ1_EN | MJ1_DIR);
    // Set pins as output
    PIOD->PIO_OER |= (MJ1_NOT_OE | MJ1_NOT_RST | MJ1_SEL | MJ1_EN | MJ1_DIR);
    // Enable internal pull up
    PIOD->PIO_PUER |= (MJ1_NOT_OE | MJ1_NOT_RST);
    // Disable internal pull up
    PIOD->PIO_PUDR |= (MJ1_SEL | MJ1_EN | MJ1_DIR);

    // MJ2 SETUP

}

void encoder_read(){
    // 1: Set !OE low, to sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(MJ1_NOT_OE);
    // 2: Set SEL low to output high byte
    PIOD->PIO_ODSR &= ~(MJ1_SEL);
    // 3: Wait approx. 20 microseconds for output to settle
    // TODO: implement delay

    // 4: Read MJ2 to get high byte

    // 5: Set SEL high to output low byte
    PIOD->PIO_ODSR |= MJ1_SEL;
    // 6: Wait approx. 20 microseconds
    // 7: Read MJ2 to get low byte
    // 8: Set !OE to high
    PIOD->PIO_ODSR |= MJ1_NOT_OE;

}