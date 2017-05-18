#include <stdlib.h>
#include <stdio.h>
#include <fsl_device_registers.h>
#include "utils.h"
//#include "gpio.h"


//1 if lock() was called last. 0 if unlock() was called last
int locked;

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
	ADC0_SC1A |= ADC_SC1_ADCH(31);
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
	PORTA_PCR4 = 0x90100; //sw3
	GPIOA_PDDR |= (0 << 4);
	PORTA_ISFR = PORT_ISFR_ISF(0x10);
	//interrupts enabled on port A
	NVIC_EnableIRQ(PORTA_IRQn);	
}

/**
 * Interrupt handler for sw3 button press. Locks the box if unlocked
 */
void PORTA_IRQHandler(void) {
	lock();
	PORTA_ISFR = PORT_ISFR_ISF(0x10);//reset interrupt
}

/**
 * Poll the sensor input. If input is over threshold then alcohol present and red LED
 * toggles and box stays locked. Otherwise toggle the green LED and unlock the box.
 * Assumes: function called from poll_microphone() in case when user assumed to be blowing
 * into the sensor (blue LED will be on in this case).
 */
void poll_sensor(void) {
	for (int i = 0; i < 500000; i++) {};
	unsigned short a = sensor_read();
	//if too much alcohol toggle red LED
	if (a > 25000) {
		LEDRed_On();
		delay();
		LEDRed_Off();
	} 
	//otherwise unlock and toggle green LED
	else {
		LEDBlue_Off();
		LEDGreen_On();
		unlock();
		LEDGreen_Off();
	}
}

/**
 * Poll the microphone input. If input is over threshold twice in 1 second interval
 * assume user blowing into module turn on blue LED to signal input was read and poll
 * the sensor
 */
void poll_microphone(void) {
	unsigned short a = 0;
	a = microphone_read();
	if (a > 64000) {
		for (int i = 0; i < 500000; i++) {};
		a = microphone_read();
		//if someone's blowing into module turn on blue LED and poll sensor 
		if (a > 64000) {
			LEDBlue_On();
			poll_sensor();
		}
		//otherwise not blowing into microphone, do nothing
		else
			LEDBlue_Off();
	} else {
		LEDBlue_Off();
	}
}

/**
 * Main entrypoint of code. Initializes the modules and ensures servo starts unlocked.
 * Then polls the microphone for input if box is locked. Box locks if sw3 is pressed on 
 * the board.
 */
int main(void) {
	//make sure servo correctly oriented in unlocked position before starting
	servo_init();
	buttonlock_setup();
	locked = 1;
	unlock();
	analogue_init();
	
	//main execution loop
	while(1) {
		if (locked) {
			poll_microphone();
		}
	}
}


