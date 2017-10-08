/********************************************************************************
* File ​Name 	  : timer.c
* File Description: This file includes the function to initialize TIMER0 to
* 					generate a precise deterministic delay of 1.8 ms
* Author		  : Subhradeep Dutta
* Date			  : 09/29/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/

#include "timer.h"

const TIMER_Init_TypeDef timerInit =
		   {
				   /** Start counting when init completed. */
				   .enable = false,
				   /** Counter shall keep running during debug halt. */
				   .debugRun = false,
				   /** Prescaling factor, if HFPER clock used. */
				   .prescale = timerPrescale1024,
				   /** Clock selection. */
				   .clkSel = timerClkSelHFPerClk,
				   /** Action on falling input edge. */
				   .fallAction = timerInputActionNone,
				   /** Action on rising input edge. */
				   .riseAction = timerInputActionNone,
				   /** Counting mode. */
				   .mode = timerModeUp,
				   /** DMA request clear on active. */
				   .dmaClrAct = false,
				   /** Select X2 or X4 quadrature decode mode (if used). */
				   .quadModeX4 = false,
				   /** Determines if only counting up or down once. */
				   .oneShot = true,
				   /** Timer start/stop/reload by other timers. */
				   .sync = false
		   };


void timer0_init()
{
	TIMER_Init(TIMER0, &timerInit);
	TIMER_IntClear(TIMER0, TIMER_IFC_ICBOF3 | TIMER_IFC_ICBOF2 | TIMER_IFC_ICBOF1 | \
				TIMER_IFC_ICBOF0 | TIMER_IFC_CC3 | TIMER_IFC_CC2 | TIMER_IFC_CC1 | \
				TIMER_IFC_CC0| TIMER_IFC_DIRCHG | TIMER_IFC_UF | TIMER_IFC_OF);
	TIMER_IntEnable(TIMER0, TIMER_IEN_OF);
	/*Setup TIMER0 TOP and COUNT values*/
	TIMER_TopSet(TIMER0, 0x22);
	TIMER_CounterSet(TIMER0, 0x00);
	NVIC_EnableIRQ(TIMER0_IRQn);
	/* Set high priority for timer0 interrupt*/
	NVIC_SetPriority( TIMER0_IRQn, 2 );
}

void TIMER0_IRQHandler(void)
{
	int intFlags;                    //Variable to store the interrupting flags
	CORE_ATOMIC_IRQ_DISABLE();                        //Disable interrupts to avoid nesting
	TIMER_IntClear(TIMER0, TIMER_IFC_ICBOF3 | TIMER_IFC_ICBOF2 | TIMER_IFC_ICBOF1 | \
				TIMER_IFC_ICBOF0 | TIMER_IFC_CC3 | TIMER_IFC_CC2 | TIMER_IFC_CC1 | \
				TIMER_IFC_CC0| TIMER_IFC_DIRCHG | TIMER_IFC_UF | TIMER_IFC_OF);
	intFlags = TIMER_IntGet(TIMER0);    //Get the interrupting flags

	if(intFlags & TIMER_IF_OF)
	{
		TIMER_IntClear(TIMER0, intFlags);
	}
	unblockSleepMode(energy_mode_1);
	CORE_ATOMIC_IRQ_ENABLE();
}
void TIMER0_start(uint32_t load_value)
{
	blockSleepMode(energy_mode_1);
	TIMER_TopSet(TIMER0, load_value);
	TIMER_CounterSet(TIMER0, 0x00);
	TIMER_Enable(TIMER0, true);
}

