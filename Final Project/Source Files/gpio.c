/********************************************************************************
* File ​Name 	  : gpio.c
* File Description: This file includes the function to initialize the GPIO pin
* 					PortF pin 4 that is connected to LED0 and set it to push
* 					pull mode with high drive strength
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/

//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"


//***********************************************************************************
// functions
//***********************************************************************************

void gpio_init(void)
{
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	GPIO_PinModeSet(SI7021ENABLE_port, SI7021ENABLE_pin, gpioModePushPull, SI7021ENABLE_default);
	GPIO_PinModeSet(SCL_port, SCL_pin, gpioModeWiredAnd, SCL_default);
	GPIO_PinModeSet(SDA_port, SDA_pin, gpioModeWiredAnd, SDA_default);

	GPIO_PinModeSet(IR1_port, IR1_pin, gpioModeInputPull, IR1_default);
	GPIO_PinModeSet(IR2_port, IR2_pin, gpioModeInputPull, IR2_default);

	GPIO_IntConfig(IR1_port, IR1_pin , false, true, true);
	GPIO_IntConfig(IR2_port, IR2_pin , false, true, true);

	GPIO_PinModeSet(ADC0_port, ADC0_pin, gpioModeDisabled, ADC0_default);
	/*Over voltage disable for analog pin PortA pin 0 for more accurate ADC measurement*/
	GPIO->P[gpioPortC].OVTDIS = 0x0001;
}

//void GPIO_ODD_IRQHandler(void)
//{
//	CORE_ATOMIC_IRQ_DISABLE();
//	GPIO_IntClear(ODD_IRQ_DISABLE);
//	/*Determine the cause of the GPIO interrupt by reading the BMA280 status register*/
//	CORE_ATOMIC_IRQ_ENABLE();
//}
//
//void GPIO_EVEN_IRQHandler(void)
//{
//	CORE_ATOMIC_IRQ_DISABLE();
//	GPIO_IntClear(EVEN_IRQ_DISABLE);
//	/*Determine the cause of the GPIO interrupt by reading the BMA280 status register*/
//	CORE_ATOMIC_IRQ_ENABLE();
//}

//	/*Enable I2C*/
//	I2Cinit();
//	I2C_Enable(I2C0, true);
//	I2C_flag = true;
//}
//else if(dataread == DOUBLE_TAP)
//{
//	/*Disable I2C*/
//	I2C_Enable(I2C0, false);
//	/*Disable PD9*/
//	GPIO_PinOutClear(SI7021ENABLE_port, SI7021ENABLE_pin);
//	I2C_flag = false;
