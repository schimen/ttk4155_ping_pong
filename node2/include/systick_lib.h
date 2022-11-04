/*
 * systick_lib.h
 *
 * Created: 03.11.2022 16:40:08
 *  Author: lineh
 */ 


#ifndef SYSTICK_LIB_H_
#define SYSTICK_LIB_H_

#include <stdio.h>

uint32_t getMillis(void);

void ms_delay(uint16_t delay);


#endif /* SYSTICK_LIB_H_ */