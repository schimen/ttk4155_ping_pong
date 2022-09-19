/*
 * adc_lib.c
 *
 * Created: 17.09.2022 16:32:24
 *  Author: lineh
 */ 

#define F_CPU 4915200

#include "adc_lib.h"
#include "util/delay.h"

#define ADC_START_ADR 0x1400

uint8_t adc_read(uint8_t channel){
	volatile char *adc = (char *) ADC_START_ADR;
	
	adc[0] = channel;
	_delay_ms(200);
	uint8_t value = adc[0];
	return value;
};
