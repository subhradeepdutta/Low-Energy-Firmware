/********************************************************************************
* File ​Name 	  : BMA280.c
* File Description: This file has the function definitions for the BMA280 functions
* Author		  : Subhradeep Dutta
* Date			  : 09/29/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/
#include <BMA280.h>

/*19MHz input from HFRCO
 * Prescaler = 1024
 * 18554 Hz Clock input
 * to Timer 0
 * Required Delay = 80ms
 * Value to be loaded = 1480
 */
#define BMA_STARTUP_DELAY (0x22)


void BMA280_enable()
{
	/* Transition from suspend mode to normal mode*/
	BMA280_RegisterWrite(USART1, PMU_LPW_REGISTER, DISABLE_SUSPEND);
	CORE_ATOMIC_IRQ_ENABLE();
	/*Wait for 1.8 ms*/
	TIMER0_start(BMA_STARTUP_DELAY);
	//sleep();
	SLEEP_Sleep();
	gpio_interrupt_init();
	GPIO_IntEnable(GPIO_INT_EN_11);

}

void BMA280_suspend()
{
	/* Transition from normal mode to suspend mode*/
	BMA280_RegisterWrite(USART1, PMU_LPW_REGISTER, SUSPEND);
}


void BMA280_init()
{
	BMA280_RegisterWrite(USART1,PMU_RANGE_REGISTER,RANGE_4G);
	BMA280_RegisterWrite(USART1, PMU_BW_REGISTER, SET125HZ);
	BMA280_RegisterWrite(USART1, INT_8_REGISTER, (TAP_QUIET_30 | TAP_DURATION_200 | TAP_SHOCK_50));
	BMA280_RegisterWrite(USART1, INT_9_REGISTER, (SAMPLES_4 | TAP_TH ));
	BMA280_RegisterWrite(USART1, INT_RST_LATCH_REGISTER, INT_ENABLE);
	BMA280_RegisterWrite(USART1, INT_MAP_0_REGISTER, MAP_TAP_INT);
	BMA280_RegisterWrite(USART1, INT_OUT_CTRL_REGISTER, INT1_TYPE);
	BMA280_RegisterWrite(USART1, INT_EN_0_REGISTER, INT_EN_TAP);

}
