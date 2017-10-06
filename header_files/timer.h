/********************************************************************************
* File ​Name 	  : timer.h
* File Description: This file includes the prototypes for the TIMER0 initialization
* Author		  : Subhradeep Dutta
* Date			  : 09/29/2017
********************************************************************************/

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "em_timer.h"
#include "main.h"
#include "em_cmu.h"
#include "gpio.h"
#include "sleep.h"

/************************************************************************************
Name: timer0_init
Description : Initializes TIMER0 to generate a delay of 1.8ms
Inputs: N/A
Returns N/A
************************************************************************************/
void timer0_init();


/************************************************************************************
Name: TIMER0_start
Description : Reloads the values to the register for reusing TIMER0 even though it is
			  setup for single shot operation
Inputs: N/A
Returns N/A
************************************************************************************/
void TIMER0_start();

#endif /* INC_TIMER_H_ */
