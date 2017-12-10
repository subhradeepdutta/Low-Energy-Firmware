/********************************************************************************
* File ​Name 	  : cmu.c
* File Description: This file has the initializations for clocks to various
* 					peripherals on the Blue gecko
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/

//***********************************************************************************
// Include files
//***********************************************************************************
#include <cmu.h>
/*Define the maximum count for the 16 bit LETIMER0*/
#define LETIMER0_MAX_VAL (65536)
/*Define the ON time here*/
#define ON_TIME (0.00)
/*Define the TOTAL time here*/
#define TOTAL_TIME (2.50)

#include "em_letimer.h"



int ON_period, TOTAL_period, Prescaler, CurrentFrequency;

void cmu_init(void)
{
	CMU_ClockEnable(cmuClock_GPCRC, true);
	CMU_ClockEnable(cmuClock_LDMA, true);
	CMU_ClockEnable(cmuClock_PRS, true);
	CMU_ClockEnable(cmuClock_RTCC, true);


	CMU_HFXOAutostartEnable(0, true, true);

	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);


	CMU_AUXHFRCOBandSet(cmuAUXHFRCOFreq_1M0Hz);
	CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
	CMU_ClockEnable(cmuClock_ADC0, true);
	CMU->ADCCTRL = CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;

	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFXO);


	CMU_ClockSelectSet(cmuClock_HF,cmuSelect_HFXO);


	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);

	/*Provide clock for peripherals*/
	CMU_ClockEnable(cmuClock_I2C0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);
	CMU_ClockEnable(cmuClock_HFLE,true);
	/*Determine the current frequency*/
	CurrentFrequency = CMU_ClockFreqGet(cmuClock_LFA);
	ON_period = CurrentFrequency * ON_TIME;
	TOTAL_period = CurrentFrequency * TOTAL_TIME;
	/*Determine the correct prescaler value*/
	while(TOTAL_period > LETIMER0_MAX_VAL)
	{
		Prescaler++;
		TOTAL_period /= 2;
		ON_period /= 2;
	}
	Prescaler++;
	/*Prescale the clock based on the frequency*/
	CMU_ClockDivSet(cmuClock_LETIMER0,Prescaler);
}

