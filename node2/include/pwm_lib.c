/*
 * pwm.c
 *
 * Created: 27.10.2022 15:13:18
 *  Author: joerg
 */ 

#include "pwm_lib.h"

void pwm_setup(){
	// PWM on pin PC18, PWM channel 6H
	
	
	// Enable peripheral clock
	PMC->PMC_PCER1 = PMC_PCER1_PID36;
	
	// Disable PIOC18
	PIOC->PIO_PDR |= PIO_PDR_P18;
	
	// Choose peripheral B
	PIOC->PIO_ABSR |= (1<<18);
	
	printf("A/B mode", (PIOC->PIO_ABSR & (uint32_t)(1<<18))); 
	
	// Disable PWM write protect for register group 0, 1, 2 and 3
	PWM->PWM_WPCR = (PWM_WPCR_WPCMD(0) | PWM_WPCR_WPRG0 | PWM_WPCR_WPRG1 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3);
	
	printf("Write protect: %d", (PWM->PWM_WPSR & 0xFFFF));
	
	// PWM clock config
	// clock A prescaler X = 128
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0x07));
	
	// Choose clock A for channel 6
	PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKA;
	
	//PWM->PWM_CH_NUM[6].PWM_SCM |= PWM_SCM_UPDM_MODE0;
	
	// Set Channel period
	// X = 128, CPRD = 13125
	PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(0x3345);
	
	// Set channel duty cycle
	PWM->PWM_CH_NUM[6].PWM_CDTY = PWM_CDTY_CDTY(0);
	
	if (PWM->PWM_SR & (1 << 6)){
		printf("PWM 6 enabled\r");
	}
	
	// Enable PWM Channel 6
	PWM->PWM_ENA = (1 << 6);
	
	if (PWM->PWM_SR & (1 << 6)){
		printf("PWM 6 enabled\r");
	}
	else{
		printf("PWM 6 not enabled, %d", (PWM->PWM_SR & 0xFF));
	}
}


void pwm_set_duty_cycle(uint8_t duty_cycle, uint8_t channel){
	// duty cycle in percentage
	if (duty_cycle > 100) {
		duty_cycle = 100;
	}
	uint32_t cprd = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t dc_cdty = (duty_cycle*cprd)/100;
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = PWM_CDTY_CDTY(dc_cdty);
	PWM->PWM_SCUC = PWM_SCUC_UPDULOCK;
	
	
}