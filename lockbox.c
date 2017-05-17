#include <stdlib.h>
#include <stdio.h>
#include <fsl_device_registers.h>
//#include "gpio.h"

void delay (void) {
	int i = 1;
	while (i<10000000) 
		i++;
}

/**
 * Initialize the servo using FTM0
 */
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

/**
 * Turns servo to certain degree corresponding to a locked box
 */
void lock (void) {
	while(1) {
		delay();
		FTM0_C7V = 320;
		delay();
		FTM0_C7V = 160;
	}
}

/**
 * Initialize the alcohol sensor and microphone using analogue to 
 * digital conversion with the ADC module
 */
void analogue_init (void) {
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	ADC0_CFG1 |= ADC_CFG1_MODE(3);
	//for alcohol sensor
	ADC0_SC1A |= ADC_SC1_ADCH(31);
	//for microphone
	ADC0_SC1B |= ADC_SC1_ADCH(31);
}

/**
 * Read data from the alcohol sensor using the ADC module.
 * Returns: value read from sensor
 */
unsigned short sensor_read (void) {
	ADC0_SC1A = 12 & ADC_SC1_ADCH_MASK;
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //wait if conversion in process
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //wait until conversion complete
	
	return ADC0_RA;//return data result register
}

/**
 * Read data from the microphone using the ADC module.
 * Returns: value read from microphone
 */
unsigned short microphone_read (void) {
	ADC0_SC1B = 14 & ADC_SC1_ADCH_MASK;
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //wait if conversion in process
	while(!(ADC0_SC1B & ADC_SC1_COCO_MASK)); //wait until conversion complete
	
	return ADC0_RB;//return data result register
}

int main(void) {
	
	
}


