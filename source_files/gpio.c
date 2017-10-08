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

bool I2C_flag = false;

//***********************************************************************************
// functions
//***********************************************************************************

void gpio_init(void)
{
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	GPIO_PinModeSet(ADC0_port, ADC0_pin, gpioModeDisabled, ADC0_default);
	/*Over voltage disable for analog pin PortA pin 0 for more accurate ADC measurement*/
	GPIO->P[gpioPortA].OVTDIS = 0x0001;

	GPIO_PinModeSet(CS_port, CS_pin, gpioModePushPull, CS_default);
	GPIO_PinModeSet(SCLK_port, SCLK_pin, gpioModePushPull, SCLK_default);
	GPIO_PinModeSet(MISO_port, MISO_pin, gpioModeInput, MISO_default);
	GPIO_PinModeSet(MOSI_port, MOSI_pin, gpioModePushPull, MOSI_default);

	GPIO_PinModeSet(SI7021ENABLE_port, SI7021ENABLE_pin, gpioModePushPull, SI7021ENABLE_default);
	GPIO_PinModeSet(SCL_port, SCL_pin, gpioModeWiredAnd, SCL_default);
	GPIO_PinModeSet(SDA_port, SDA_pin, gpioModeWiredAnd, SDA_default);

}

void gpio_interrupt_init()
{
	/*Configure PD11 as GPIO interrupt*/
	GPIO_IntClear(GPIO_IntGet());
	GPIO_PinModeSet(GPIOINT_port, GPIOINT_pin, gpioModeInput, GPIOINT_default);
	GPIO_IntConfig(GPIOINT_port, GPIOINT_pin , true, false, false);
	GPIO_IntEnable(GPIO_INT_EN_11);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void GPIO_ODD_IRQHandler(void)
{
CORE_ATOMIC_IRQ_DISABLE();
GPIO_IntClear(DISABLE_INT11);
/*Determine the cause of the GPIO interrupt by reading the BMA280 status register*/
uint8_t dataread = BMA280_RegisterRead(USART1, INT_STATUS_0_REGISTER);
if(dataread == SINGLE_TAP)
{

	I2C_Enable(I2C0, true);
	I2C_flag = true;
}
else if(dataread == DOUBLE_TAP)
{
	I2C_Enable(I2C0, false);
	I2C_flag = false;
}
  GPIO_IntEnable(GPIO_INT_EN_11);
  CORE_ATOMIC_IRQ_ENABLE();

}
