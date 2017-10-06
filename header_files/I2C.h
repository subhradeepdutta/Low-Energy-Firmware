/*
 * I2C.h
 *
 *  Created on: Oct 4, 2017
 *      Author: subhr
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_


#define SI7021_ADDRESS (0x40)
#define READ_COMMAND (0x01)
#define WRITE_COMMAND (0x00)
#define READTEMP_HOLD_MASTER_MODE (0xE3)
#define DEFAULT_TEMPERATURE (30)

#include "em_i2c.h"
#include "gpio.h"


void I2Cinit();
uint16_t I2C_read(uint8_t register_address);

void I2C_start() __attribute__((always_inline));
void I2C_stop() __attribute__((always_inline));

#endif /* INC_I2C_H_ */
