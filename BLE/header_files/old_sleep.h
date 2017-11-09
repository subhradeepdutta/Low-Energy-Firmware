/********************************************************************************
* File ​Name 	  : sleep.h
* File Description: This file has enum definitions and the prototypes for sleep
* 					functions
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
********************************************************************************/
/*Include guard */
#ifndef SLEEP_H_
#define SLEEP_H_

typedef enum sleepstate_enum_t
{
	energy_mode_0,
	energy_mode_1,
	energy_mode_2,
	energy_mode_3,
	energy_mode_4
}sleepstate_enum;

/************************************************************************************
Name: sleep
Description : Based on the value of sleep block counter this function decides which
			  energy mode to sleep in
Inputs: N/A
Returns N/A
************************************************************************************/
void sleep(void);

/************************************************************************************
Name: blockSleepMode
Description : Blocks the sleep mode to a lowest possible energy mode where the
			  desired peripherals are still available
Inputs: The lowest possible energy mode where the desired peripheral is available
Returns N/A
************************************************************************************/
void blockSleepMode(sleepstate_enum minimumMode);

/************************************************************************************
Name: unblockSleepMode
Description : Unblocks the sleep mode so that system can go into deeper sleep modes
Inputs: The lowest possible energy mode where the system was blocked to allow deeper
		sleep modes
Returns N/A
************************************************************************************/
void unblockSleepMode(sleepstate_enum minimumMode);

#endif /* INC_SLEEP_H_ */
