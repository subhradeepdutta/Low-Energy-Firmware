/********************************************************************************
* File ​Name 	  : SPI.h
* File Description: This file includes the prototypes for the SPI initialization
* 					and SPI read and write functions
* Author		  : Subhradeep Dutta
* Date			  : 09/29/2017
********************************************************************************/

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "em_usart.h"


/************************************************************************************
Name: SPI_init
Description : Initializes the USART1 for SPI communication
Inputs: N/A
Returns N/A
************************************************************************************/
void SPI_init();

/************************************************************************************
Name: BMA280_RegisterRead
Source : This function uses example code from the following Silicon Labs community
		 page
		http://community.silabs.com/t5/Bluetooth-Wi-Fi/Using-accelerometer-in-BGM111-
		BMG113-dev-kit-with-C-SDK/m-p/176315#M13069
Description : Reads a given register from the BMA280 over SPI bus
Inputs: USART and the register address to be read
Returns: The register value read over SPI bus
************************************************************************************/
uint8_t BMA280_RegisterRead(USART_TypeDef *usart, uint8_t address);

/************************************************************************************
Name: BMA280_RegisterWrite
Source : This function uses example code from the following Silicon Labs community
		 page
		http://community.silabs.com/t5/Bluetooth-Wi-Fi/SPI-write-for-BMA280-
		accelerometer/td-p/179732
Description : Writes to a given register on the BMA280 over SPI bus
Inputs: USART and the register address and the data to be written
Returns N/A
************************************************************************************/
void BMA280_RegisterWrite(USART_TypeDef *usart, uint8_t address, uint8_t data);

#endif /* INC_SPI_H_ */
