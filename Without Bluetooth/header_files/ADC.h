/********************************************************************************
* File ​Name 	  : ADC.h
* File Description: This file has prototypes for all the ADC functions
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
********************************************************************************/
#ifndef INC_ADC_H_
#define INC_ADC_H_

extern int ON_period, TOTAL_period, Prescaler, CurrentFrequency;

#include "em_adc.h"
#include "em_core.h"
#include "gpio.h"
#include "sleep.h"
#include <stdbool.h>
#include "BMA280.h"

extern int32_t global_temperature;

/************************************************************************************
Name: swap_threshold
Description : This function swaps the threshold values in the ADC0 CMPTHR registers.
			  This function is forced as inline.
Inputs: N/A
Returns N/A
************************************************************************************/
void swap_threshold() __attribute__((always_inline));


/************************************************************************************
Name: ADC_SYNC_To_ASYNC
Description : This function changes the mode for the ADC from synchronous to
			  asynchronous
Inputs: N/A
Returns N/A
************************************************************************************/
void ADC_SYNC_To_ASYNC();


/************************************************************************************
Name: ADC_ASYNC_To_SYNC
Description : This function changes the mode for the ADC from asynchronous to
			  synchronous
Inputs: N/A
Returns N/A
************************************************************************************/
void ADC_ASYNC_To_SYNC();

/************************************************************************************
Name: ADC_Initialize
Description : This function intializes the ADC with all the required parameters
Inputs: N/A
Returns N/A
************************************************************************************/
void ADC_Initialize();

#endif /* INC_ADC_H_ */
