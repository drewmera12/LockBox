#include <stdlib.h>
#include <stdio.h>
#include <fsl_device_registers.h>
//#include "gpio.h"

void delay (void) {
	int i = 1;
	while (i<10000000) 
		i++;
	
}

void servo_init (void) {
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA_PCR2 = PORT_PCR_MUX(3);
	
	FTM0_CNTIN = 0;
	FTM0_SC |= 0x004F;
	FTM0_MOD = 3200;
	
	FTM0_C7SC |= 0x0028;
	
	FTM0_C7V = 160;
}

void lock (void) {
	while(1) {
		delay();
		FTM0_C7V = 320;
		delay();
		FTM0_C7V = 160;
	}
}

int main(void) {
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	ADC0_CFG1 |= ADC_CFG1_MODE(3);
	ADC0_SC1A |= ADC_SC1_ADCH(31);
	
	//read	
	ADC0_SC1A = 12 & ADC_SC1_ADCH_MASK;
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));
	
	unsigned short a = 0;
	a = ADC0_RA;
	
	if (a>10000)
		while(1);
	else
		while(1);
	
	
	
	
	
	

	
//	
//	PORTD_PCR0 = PORT_PCR_MUX(4);
//	
//	FTM0_MODE |= FTM_MODE_WPDIS_MASK;
//	
//	FTM0_MODE &= ~1;
//	
//	FTM0_QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
//	
//	FTM0_CNT = 0x0;
//	FTM0_MOD = 3200;
//	FTM0_CNTIN = 0;
//	
//	//counter
//	FTM0_C0SC |= FTM_CnSC_ELSB_MASK;
//	FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK;
//	FTM0_C0SC |= FTM_CnSC_MSB_MASK;
//	
//	
//	FTM0_SC = 0;
//	FTM0_C0V=FTM0_MOD;
//	
//	
//	FTM0_SC = FTM_SC_CLKS(1);
//	FTM0_SC |= FTM_SC_PS(111);
//	
//	FTM0_SC |= FTM_SC_TOIE_MASK;
//	
//	
//	FTM0_C0V = 320;
//	

	
	
	//FTM0_C1V = 0;
	
	
}


