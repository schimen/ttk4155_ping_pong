/*
 * pwm.c
 *
 * Created: 27.10.2022 15:13:18
 *  Author: joerg
 */ 

#include "pwm_lib.h"

int pwm_setup(){
	printf("Set up pwm\n");
	// PWM on pin PC18, PWM channel 6H
	
	// Enable peripheral clock
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	
	// Check that
	while((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0);
	printf("Clock is now set\n");

	// Disable PIOC18
	PIOC->PIO_PDR |= PIO_PDR_P18;

	// Check that PC18 was set to peripheral output (bit PSR bit 18 set to 0)
	if (PIOC->PIO_PSR & PIO_PSR_P18) {
		printf("PC18 could not be set to peripheral output\n");
		return -1;
	}
	
	// Choose peripheral B
	PIOC->PIO_ABSR |= PIO_ABSR_P18;
	
	printf("A/B mode %d\n", ((PIOC->PIO_ABSR & PIO_ABSR_P18) >> 18)); 
	

	// Disable PWM write protect for register group 0, 1, 2, 3, 4 and 5
	PWM->PWM_WPCR = (PWM_WPCR_WPCMD(0) | (0x3F << 1));

	// Disble pwm chnnel 6
	PWM->PWM_DIS = (1 << 6);
	
	// PWM clock config
	// clock A prescaler X = 128
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0x07));
	
	// Choose clock A for channel 6
	PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKA;
	
	// Set sync mode
	PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0;
	
	// Set Channel period
	// X = 128, CPRD = 13125
	PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(0x3345);
	
	// Set channel duty cycle
	PWM->PWM_CH_NUM[6].PWM_CDTY = PWM_CDTY_CDTY(0);
	
	// Enable PWM Channel 6
	PWM->PWM_ENA = (1 << 6);
	if (PWM->PWM_SR & (1 << 6)){
		printf("PWM 6 enabled\r");
		return 0;
	}
	else{
		printf("PWM 6 not enabled, %d", (PWM->PWM_SR & 0xFF));
		return -1;
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