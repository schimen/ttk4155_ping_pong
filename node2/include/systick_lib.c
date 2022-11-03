/*
 * systick_lib.c
 *
 * Created: 03.11.2022 16:39:39
 *  Author: lineh
 */ 
#include "systick_lib.h"
volatile uint32_t ms_ticks = 0;

void SysTick_Handler(void){
	ms_ticks++;
}

 uint32_t getMillis(void){return ms_ticks;}
	
void ms_delay(uint16_t delay){
	uint32_t prevMillis = getMillis();
	while (getMillis() <= prevMillis + delay){}
}


