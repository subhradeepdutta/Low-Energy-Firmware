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
#include "main.h"
#include "em_cmu.h"
#include "I2C.h"

extern int ON_period, TOTAL_period, Prescaler, CurrentFrequency;
extern bool I2C_flag;
extern int32_t global_temperature;

/************************************************************************************
Name: le_timer_init
Description : Initializes the LETIMER0 and loads the appropriate values in the
			  corresponding registers
Inputs: N/A
Returns N/A
************************************************************************************/

void le_timer_init();

/************************************************************************************
Name: LED_ON
Description : A static inline function that turns on the LED
Inputs: N/A
Returns N/A
************************************************************************************/

void LED_ON() __attribute__((always_inline));

/************************************************************************************
Name: LED_OFF
Description : A static inline function that turns off the LED
Inputs: N/A
Returns N/A
************************************************************************************/

void LED_OFF() __attribute__((always_inline));


#endif /* INC_LETIMER_H_ */
