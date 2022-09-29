/*
 * adc_lib.c
 *
 * Created: 17.09.2022 16:32:24
 *  Author: lineh
 */ 
#include "adc_lib.h"

void adc_read(void)
 {
	volatile char *adc = (char *)ADC_START_ADR;
	
	// Hardwire mode: sample all channels
	adc[0] = (uint8_t)0; 
	_delay_ms(10);
	
	// Read result from all analog channels
	joystick.y_adc = adc[0];
	joystick.x_adc = adc[0];
	slider.left_adc = adc[0];
	slider.right_adc = adc[0];

}


