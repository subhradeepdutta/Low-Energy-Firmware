/********************************************************************************
* File ​Name 	  : gpio.h
* File Description: This file includes the declaration of LED0 and GPIO function
* 					prototypes
* Author		  : Subhradeep Dutta
* Date			  : 09/13/2017
********************************************************************************/

/*Include guard */
#ifndef GPIO_H_
#define GPIO_H_

//***********************************************************************************
// Include files
//***********************************************************************************
#include "../inc/main.h"
#include "em_gpio.h"
#include "BMA280.h"
#include "I2C.h"


// LED0 pin
#define	LED0_port gpioPortF
#define LED0_pin (4)
#define LED0_default	false 	// off

// LED0 pin
#define	LED1_port gpioPortF
#define LED1_pin (5)
#define LED1_default	false 	// off

// ADC pin
#define	ADC0_port gpioPortA
#define ADC0_pin (0)
#define ADC0_default	false 	// off

// CS pin
#define	CS_port gpioPortC
#define CS_pin (9)
#define CS_default	true 	// off

// SCLK pin
#define	SCLK_port gpioPortC
#define SCLK_pin (8)
#define SCLK_default	true 	// off

// MISO pin
#define	MISO_port gpioPortC
#define MISO_pin (7)
#define MISO_default	false 	// off

// MOSI pin
#define	MOSI_port gpioPortC
#define MOSI_pin (6)
#define MOSI_default	true 	// true

// PD11 GPIO Interrupt pin
#define	GPIOINT_port gpioPortD
#define GPIOINT_pin (11)
#define GPIOINT_default	false 	// off

// PD9 Enable Temperature Humidity Sensor pin
#define	SI7021ENABLE_port gpioPortD
#define SI7021ENABLE_pin (9)
#define SI7021ENABLE_default	false 	// off

// PC11 SDA pin
#define	SDA_port gpioPortC
#define SDA_pin (11)
#define SDA_default	true 	// true

// PC10 SCL pin
#define	SCL_port gpioPortC
#define SCL_pin (10)
#define SCL_default	true 	// true

#define GPIO_INT_EN_11 0x00000800

#define DISABLE_INT11 (1<<11)
//***********************************************************************************
// function prototypes
//***********************************************************************************

/************************************************************************************
Name: gpio_init
Description : Initializes GPIO pins as specified in the definition
Inputs: N/A
Returns N/A
************************************************************************************/
void gpio_init(void);

/************************************************************************************
Name: gpio_interrupt_init
Description : Configures GPIO pin D11 for external interrupts from rising edge signals
Inputs: N/A
Returns N/A
************************************************************************************/
void gpio_interrupt_init();
#endif
