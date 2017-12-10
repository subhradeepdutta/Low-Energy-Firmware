/********************************************************************************
* File ​Name 	  : letimer.h
* File Description: This file has prototype for LETIMER0 initialization
* 					function
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
********************************************************************************/
/*Include guard */
#ifndef LETIMER_H_
#define LETIMER_H_

#include "em_letimer.h"
#include "em_cmu.h"
#include "I2C.h"
#include "native_gecko.h"
#include "em_core.h"
#include "cmu.h"

extern int ON_period, TOTAL_period, Prescaler, CurrentFrequency;

/************************************************************************************
Name: le_timer_init
Description : Initializes the LETIMER0 and loads the appropriate values in the
			  corresponding registers
Inputs: N/A
Returns N/A
************************************************************************************/

void le_timer_init();


#endif /* INC_LETIMER_H_ */
