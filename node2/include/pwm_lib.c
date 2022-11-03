/*
 * pwm.c
 *
 * Created: 27.10.2022 15:13:18
 *  Author: joerg
 */ 

#include "pwm_lib.h"

int pwm_setup(){
	// Enable PWM in PMC and wait for register to be set
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	while((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0);
	// Disable PIOC18
	PIOC->PIO_PDR |= PIO_PDR_P18;
	// Choose peripheral B
	PIOC->PIO_ABSR |= PIO_ABSR_P18;
	// Disable PWM write protect for register group 0, 1, 2, 3, 4 and 5
	PWM->PWM_WPCR = (PWM_WPCR_WPCMD(0) | (0x3F << 1));
	// Disable PWM channel 6
	PWM->PWM_DIS = (1 << 6);
	// PWM clock config
	// clock A prescaler X = 128
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0x07));
	// Choose clock A for channel 6 and invert output
	PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
	// Set sync mode
	PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0;
	// Set Channel period
	// X = 128, CPRD = 13125
	PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(0x3345);
	// Set channel duty cycle
	PWM->PWM_CH_NUM[6].PWM_CDTY = PWM_CDTY_CDTY(0);
	// Enable PWM Channel 6
	PWM->PWM_ENA = (1 << 6);
	// Wait for PWM channel enable
	while((PWM->PWM_SR & (1 << 6)) == 0);
	// Return 0 (success) when PWM has been enabled
	return 0;
}

// Set the pulse value of the PWM
static inline void pwm_set_pulse(uint32_t pulse, uint8_t channel) {
	// Set CDTYUPD register to update PWM pulse
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = PWM_CDTY_CDTY(pulse);
	// Trigger duty cycle update on next period
	PWM->PWM_SCUC = PWM_SCUC_UPDULOCK;
}

void pwm_set_duty_cycle(uint8_t duty_cycle, uint8_t channel) {
	// duty cycle in percentage
	if (duty_cycle > 100) {
		duty_cycle = 100;
	}
	uint32_t cprd = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t pulse = (duty_cycle*cprd)/100;
	pwm_set_pulse(pulse, channel);
}

void servo_set_pos(uint8_t pos, uint8_t channel) {
	// duty cycle in percentage
	if (pos > 180) {
		pos = 180;
	}
	uint32_t cprd = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t cprd_top = cprd/10;
	uint32_t cprd_bottom = cprd_top/2;
	uint32_t cprd_range = cprd_top - cprd_bottom;
	uint32_t pulse = ((pos*cprd_range)/180) + cprd_bottom;
	pwm_set_pulse(pulse, channel);
}