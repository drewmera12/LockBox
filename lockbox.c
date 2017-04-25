#include <stdlib.h>
#include <fsl_device_registers.h>
//#include "gpio.h"



int main(void) {
	SIM_SCGC5 = SIM_SCGC5_PORTD_MASK;
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	
	PORTD_PCR0 = PORT_PCR_MUX(4);
	
	FTM0_SC |= 0x004F;
	FTM0_MOD = 3200;
	
	while(1) {
		FTM0_C0V=20;
	}
	
	
	//FTM0_C1V = 0;
	
	
}