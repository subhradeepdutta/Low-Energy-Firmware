/********************************************************************************
* File ​Name 	  : I2C.h
* File Description: This file includes the declaration of I2C initialization
* 					functions and to read the temperature over I2C
* Author		  : Subhradeep Dutta
* Date			  : 10/07/2017
********************************************************************************/

#ifndef INC_I2C_H_
#define INC_I2C_H_


#define SI7021_ADDRESS (0x40)
#define READ_COMMAND (0x01)
#define WRITE_COMMAND (0x00)
#define READTEMP_HOLD_MASTER_MODE (0xE3)
#define DEFAULT_TEMPERATURE (15)

#include "em_i2c.h"
#include "gpio.h"

/************************************************************************************
Name: I2Cinit
Description : Initializes I2C0 to communicate with SI7021
Inputs: N/A
Returns: N/A
************************************************************************************/
void I2Cinit();

/************************************************************************************
Name: I2C_read
Description : Sends a command to read the temperature from the SI7021 sensor
Inputs: 0xE3 Command to request and read back temperature in hold master mode
Returns: The 16 bit temperature code
************************************************************************************/
uint16_t I2C_read(uint8_t register_address);

/************************************************************************************
Name: I2C_start
Description : Inline function to send a start condition on the I2C bus
Inputs: N/A
Returns: N/A
************************************************************************************/
void I2C_start() __attribute__((always_inline));

/************************************************************************************
Name: I2C_stop
Description : Inline function to send a stop condition on the I2C bus
Inputs: N/A
Returns: N/A
************************************************************************************/
void I2C_stop() __attribute__((always_inline));

#endif /* INC_I2C_H_ */
