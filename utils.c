#include <fsl_device_registers.h>
#include "utils.h"

/**
 * Busy-wait delay function. Used in servo functions
 */
void delay (void) {
	int i = 1;
	while (i<100000) 
		i++;
}

/**
 * Turn the Red LED on
 */
void Led_RedOn() {
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
  PORTB->PCR[22] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
  PTB->PDDR = (1 << 22);
  PTB->PCOR = (1 << 22);
	return;
}

/**
 * Turn the Red LED off
 * Requires: the Red LED is turned on
 */
void Led_RedOff() {
  PTB->PSOR = (1 << 22);
	return;
}

/**
 * Turn the Blue LED on
 */
void Led_BlueOn() {
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
  PORTB->PCR[21] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
  PTB->PDDR = (1 << 21);
  PTB->PCOR = (1 << 21);
	return;
}

/**
 * Turn the Blue LED off
 * Requires: the Blue LED is turned on
 */
void Led_BlueOff() {
  PTB->PSOR = (1 << 21);
	return;
}

/**
 * Turn the Green LED on
 */
void Led_GreenOn() {
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port B
  PORTE->PCR[26] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
  PTE->PDDR = (1 << 26);
  PTE->PCOR = (1 << 26);
	return;
}

/**
 * Turn the Green LED off
 * Requires: the Green LED is turned on
 */
void Led_GreenOff() {
	PTE->PSOR = (1 << 26);
	return;
}
