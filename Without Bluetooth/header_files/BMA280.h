/********************************************************************************
* File ​Name 	  : BMA280.h
* File Description: This file has prototypes for all the BMA280 functions
* Author		  : Subhradeep Dutta
* Date			  : 09/29/2017
********************************************************************************/

#ifndef INC_BMA280_H_
#define INC_BMA280_H_

#include "SPI.h"
#include "timer.h"

#define SUSPEND (0x80)
#define DISABLE_SUSPEND  (0x00)
#define SET125HZ (0x0C)
#define PMU_LPW_REGISTER (0x11)
#define PMU_BW_REGISTER (0x10)
#define PMU_RANGE_REGISTER (0x0F)
#define RANGE_4G (0x05)
#define INT_8_REGISTER (0x2A)
#define TAP_QUIET_30 (0x00)
#define INT_9_REGISTER (0x2B)
#define SAMPLES_4 (0x40)
#define TAP_DURATION_200 (0x03)
#define TAP_SHOCK_50 (0x00)
#define TAP_TH (0x02)
#define INT_RST_LATCH_REGISTER (0x21)
#define INT_ENABLE (0x0C)
#define INT_MAP_0_REGISTER (0x19)
#define MAP_TAP_INT (0x30)
#define INT_OUT_CTRL_REGISTER (0x20)
#define INT1_TYPE (0x01)
#define INT_EN_0_REGISTER (0x16)
#define INT_EN_TAP (0x30)
#define INT_STATUS_0_REGISTER (0x09)
#define SINGLE_TAP (0x20)
#define DOUBLE_TAP (0x10)

/************************************************************************************
Name: BMA280_enable
Description : This function forces the BMA280 from suspend mode to normal mode
Inputs: N/A
Returns N/A
************************************************************************************/
void BMA280_enable();

/************************************************************************************
Name: BMA280_suspend
Description : This function forces the BMA280 from normal mode to suspend mode
Inputs: N/A
Returns N/A
************************************************************************************/
void BMA280_suspend();

/************************************************************************************
Name: BMA280_init
Description : This function initializes the BMA280 with the given configuration
			  and sets it up for single and double tap interrupts through INT1
Inputs: N/A
Returns N/A
************************************************************************************/
void BMA280_init();

#endif /* INC_BMA280_H_ */
