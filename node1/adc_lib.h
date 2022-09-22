/*
 * adc_lib.h
 *
 * Created: 17.09.2022 16:33:21
 *  Author: lineh
 */ 
#ifndef ADC_LIB_H_
#define ADC_LIB_H_

#define F_CPU 4915200

#include <stdio.h>
#include "sram_lib.h"
#include "util/delay.h"

/* PUBLIC VARIABLES */
#define ADC_START_ADR 0x1400

 struct ADC_Values {
	 uint8_t  x_val;
	 uint8_t  y_val;
	 uint8_t  left_slider;
	 uint8_t  right_slider;
 }adc_result;

/* PUBLIC FUNCTIONS */
void adc_read(void);

#endif