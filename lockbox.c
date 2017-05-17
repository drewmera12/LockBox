#include <stdlib.h>
#include <stdio.h>
#include <fsl_device_registers.h>
//#include "gpio.h"


//1 if lock() was called last. 0 if unlock() was called last
int locked;

/**
 * Busy-wait delay function. Used in servo functions
 */
void delay (void) {
	int i = 1;
	while (i<100000) 
		i++;
}

/**
 * Initialize the servo using FTM0
 */
void servo_init (void) {
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA_PCR2 = PORT_PCR_MUX(3);//port a2
	
	FTM0_CNTIN = 0;
	FTM0_SC |= 0x004F;
	FTM0_MOD = 3200; //set period of ~20ms
	
	FTM0_C7SC |= 0x0028;
}

/**
 * Turns servo 90 degrees corresponding to locking box
 * Requires: locked = 0
 */
void lock (void) {
	if (!locked) {
		FTM0_C7V = 320;
		locked = 1;
		delay();
	}
}

/**
 * Turns servo 90 degrees (opposite direction of lock())
 * corresponding to unlocking box
 * Requires: locked = 1
 */
void unlock(void) {
	if (locked) {
		FTM0_C7V = 160;
		locked = 0;
		delay();
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
	//ADC0_SC1B |= ADC_SC1_ADCH(31);
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
	ADC0_SC1A = 13 & ADC_SC1_ADCH_MASK;
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //wait if conversion in process
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //wait until conversion complete
	
	return ADC0_RA;//return data result register
}

/**
 * Setup interrupts for sw3 button press
 */
void buttonlock_setup(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA_PCR4 = 0x90100;
	GPIOA_PDDR |= (0 << 4);
	PORTA_ISFR = PORT_ISFR_ISF(0x10);
	NVIC_EnableIRQ(PORTA_IRQn);	
}

/**
 * Interrupt handler for sw3 button press. Locks the box if unlocked
 */
void PORTA_IRQHandler(void) {
	lock();
	PORTA_ISFR = PORT_ISFR_ISF(0x10);
}

int main(void) {
	//make sure servo correctly oriented in unlocked position before starting
	servo_init();
	buttonlock_setup();
	locked = 1;
	unlock();
	analogue_init();
	
	//infinite loop to test sw3 interrupt
	unsigned short a = 0;
	while(1) {
		a = microphone_read();
		delay();
		
	}
	
	//for loop to test servo functionality
	/*for (int i =0; i < 5; i++) {
		lock();
		unlock();		
	}*/
}


