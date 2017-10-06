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

int ON_period, TOTAL_period, Prescaler, CurrentFrequency;

void cmu_init(void)
{
	/*Set the band for HFRCO*/
	CMU_HFRCOBandSet(cmuHFRCOFreq_19M0Hz);
	/*Disable autostart for HFXO*/
	CMU_HFXOAutostartEnable(0, false, false);
	/*Enable HFRCO*/
	CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);

	CMU_AUXHFRCOBandSet(cmuAUXHFRCOFreq_1M0Hz);
	CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
	CMU_ClockEnable(cmuClock_ADC0, true);
	CMU->ADCCTRL = CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;

	CMU_ClockSelectSet(cmuClock_HF,cmuSelect_HFRCO);
	/*With HFRCO turned ON, it is now safe to disable HFXO*/
	CMU_OscillatorEnable(cmuOsc_HFXO, false, false);
#ifdef EMTHREE
	/*Using ULFRCO for EM3 energy mode for LETIMER0*/
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);
#else
	/*Using LFXO when in EM0, EM1, EM2*/
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
#endif
	/*Provide clock for peripherals*/
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_USART1, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);
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

