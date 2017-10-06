/********************************************************************************
* File ​Name 	  : cmu.h
* File Description: This file has prototype for clock initialization
* 					function
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
********************************************************************************/
/*Include guard */
#ifndef CMU_H
#define CMU_H

//***********************************************************************************
// Include files
//***********************************************************************************
#include "../inc/main.h"
#include "em_cmu.h"


//***********************************************************************************
// function prototypes
//***********************************************************************************

/************************************************************************************
Name: cmu_init
Description : This function initializes the various clocks for the peripherals on the
			  gecko
Inputs: N/A
Returns N/A
************************************************************************************/
void cmu_init(void);

#endif
