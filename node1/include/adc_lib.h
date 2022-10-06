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
#include "util/delay.h"
#include "js_slider.h"

#define ADC_START_ADR 0x1400

void adc_read(void);

#endif