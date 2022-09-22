/*
 * adc_lib.c
 *
 * Created: 17.09.2022 16:32:24
 *  Author: lineh
 */ 
#include "adc_lib.h"


void adc_read(void) {
	volatile char *adc = (char *)ADC_START_ADR;
	
	adc[0] = (uint8_t)0; //Hardwire mode: sample all channels
	
	_delay_ms(10);
	
	//Read result from all analog channels
	adc_result.x_val = adc[0];
	adc_result.y_val = adc[0];
	adc_result.left_slider = adc[0];
	adc_result.right_slider = adc[0];
};


