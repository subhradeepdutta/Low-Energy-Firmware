/********************************************************************************
* File ​Name 	  : letimer.c
* File Description: This file has the interrupt handler for LETIMER0 interrupt
* 					the initialization of LETIMER0
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/

#include "letimer.h"
#include "gpio.h"
#include "cmu.h"


float temperature;
const LETIMER_Init_TypeDef letimerInit =
		   {
		       .enable         = false,                 //Start counting when init completed
		       .debugRun       = false,                 //Counter shall not keep running during debug halt
		       .comp0Top       = true,                 //Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP
		       .bufTop         = false,                //Don't load COMP1 into COMP0 when REP0 reaches 0
		       .out0Pol        = 0,                    //Idle value for output 0.
		       .out1Pol        = 0,                    //Idle value for output 1.
		       .ufoa0          = letimerUFOANone,      //PWM output on output 0
		       .ufoa1          = letimerUFOANone,      //Pulse output on output 1
		       .repMode        = letimerRepeatFree     //Count until stopped
		   };

void LETIMER0_IRQHandler(void)
{
   int intFlags;                    //Variable to store the interrupting flags
   CORE_ATOMIC_IRQ_DISABLE();              //Disable interrupts to avoid nesting
   intFlags = LETIMER_IntGet(LETIMER0);    //Get the interrupting flags
   LETIMER_IntClear(LETIMER0, intFlags);   //Clear the interrupting flags

   if(intFlags & LETIMER_IF_COMP1)
   {
	   gecko_external_signal(0x00000001);
   }
   CORE_ATOMIC_IRQ_ENABLE();                         //Re-enable interrupts
}




void le_timer_init()
{
		LETIMER_Init(LETIMER0, &letimerInit);
		LETIMER_CompareSet(LETIMER0, 0, TOTAL_period);
		LETIMER_CompareSet(LETIMER0, 1, ON_period);

		while(LETIMER0->SYNCBUSY);

		LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1);
		LETIMER_IntEnable(LETIMER0 , LETIMER_IEN_COMP1 | LETIMER_IEN_UF);
		NVIC_EnableIRQ(LETIMER0_IRQn);
		CORE_ATOMIC_IRQ_ENABLE();

}
