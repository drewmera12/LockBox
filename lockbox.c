#include <stdlib.h>
#include <fsl_device_registers.h>
//#include "gpio.h"



int main(void) {
	SIM_SCGC5 = SIM_SCGC5_PORTD_MASK;
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	
	PORTD_PCR0 = PORT_PCR_MUX(4);
	
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;
	
	FTM0_MODE &= ~1;
	
	FTM0_QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
	
	FTM0_CNT = 0x0;
	FTM0_MOD = 3200;
	FTM0_CNTIN = 0;
	
	//counter
	FTM0_C0SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK;
	FTM0_C0SC |= FTM_CnSC_MSB_MASK;
	
	
	FTM0_SC = 0x004f;
	FTM0_C0V=FTM0_MOD*(.002*.01);
	
	//FTM0_SC = FTM0_SC_CLKS(1);
	

	
	
	//FTM0_C1V = 0;
	
	
}