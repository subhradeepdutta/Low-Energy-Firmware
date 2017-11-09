/********************************************************************************
* File ​Name 	  : ADC.c
* File Description: This file has the function definitions for the ADC
* Author		  : Subhradeep Dutta
* Date			  : 09/20/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/
#include "adc.h"
#include "letimer.h"
#define NORTHLOWER (3400)
#define NORTHUPPER (3600)
#define SOUTHLOWER (1900)
#define SOUTHUPPER (2100)
#define EASTUPPER (3200)
#define EASTLOWER (2950)
#define WESTUPPER (2600)
#define WESTLOWER (2300)
#define DEPRESSED (100)
#define INCREMENT (0.5)
#define LFXO_F (16384)
#define ULFRCO_F (1000)
#define OPERATING_FREQ (1000)
#define NO_OF_SAMPLES (200)
#define ERROR (0.04)
#define TEMPERATURE_LOWER_LIMIT (0)
#define TEMPERATURE_UPPER_LIMIT (100)

bool button_pressed = false;
bool normalmode = false;

uint16_t adc_cmp_gt = 0x0014;
uint16_t adc_cmp_lt = 0x0E74;                 /* ~0.5V for 3.3V AVDD */

float on_period = ON_TIME;
float total_period = TOTAL_TIME;


inline void swap_threshold()
{
	uint16_t temp;
	/*Temporary variable to swap the values*/
	temp = adc_cmp_gt;
	adc_cmp_gt = adc_cmp_lt;
	adc_cmp_lt = temp;
	/*Write back the swapped values to the registers*/
	ADC0->CMPTHR = _ADC_CMPTHR_RESETVALUE;
	ADC0->CMPTHR = (adc_cmp_gt << _ADC_CMPTHR_ADGT_SHIFT) +
			 	                 (adc_cmp_lt << _ADC_CMPTHR_ADLT_SHIFT);
}

void ADC0_IRQHandler(void)
{
	   /*Variable to store the interrupting flags*/
	   uint32_t intFlags;
	   /*Disable interrupts to avoid nesting*/
	   CORE_ATOMIC_IRQ_DISABLE();
	   intFlags = ADC_IntGet(ADC0);
	   uint32_t ADC_data = ADC_DataSingleGet(ADC0);
	   /*Clear FIFO*/
	   ADC0->SINGLEFIFOCLEAR = ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR;
	   /*Clear the interrupting flags*/
	   ADC_IntClear(ADC0, intFlags);
	   if(intFlags & ADC_IF_SINGLECMP)
	   {
		   /*NORTH*/
		  if((ADC_data > NORTHLOWER && ADC_data < NORTHUPPER) && (!button_pressed))
		  {
			  button_pressed = true;
			  swap_threshold();
			  /*Set boolean flag to true once so that BMA280 enters normal mode after enabling interrupts*/
			  normalmode = true;
		  }
		  /*SOUTH*/
		  else if((ADC_data > SOUTHLOWER && ADC_data < SOUTHUPPER) && (!button_pressed))
		  {
			  button_pressed = true;
			  swap_threshold();
			  /*Put BMA280 in suspend mode to save power*/
			  BMA280_suspend();
			  /*Turn LED1 off*/
			  GPIO_PinOutClear(LED1_port, LED1_pin);
		  }
		  /*WEST*/
		  else if((ADC_data > WESTLOWER && ADC_data < WESTUPPER) && (!button_pressed))
		  {
			  button_pressed = true;
			  swap_threshold();
			  global_temperature = global_temperature - 5;

		  }
		  /*EAST*/
		  else if((ADC_data > EASTLOWER && ADC_data < EASTUPPER) && (!button_pressed))
		  {
			  button_pressed = true;
			  swap_threshold();
			  global_temperature = global_temperature + 5;
		  }
		  /*DEBOUNCING*/
		  else if(button_pressed)
		  {
			 button_pressed = false;
			 swap_threshold();
		  }
		 }
	   CORE_ATOMIC_IRQ_ENABLE();
	   if(normalmode)
	  {
		  /*Enable BMA280 now once global interrupts are back on */
		  BMA280_enable();
		  normalmode = false;
	  }
}

void ADC_SYNC_To_ASYNC()
{
		NVIC_DisableIRQ(ADC0_IRQn);
		ADC_IntDisable(ADC0, ADC_IFC_PROGERR | ADC_IFC_VREFOV | ADC_IFC_SCANCMP | ADC_IFC_SINGLECMP | ADC_IFC_SCANUF | ADC_IFC_SINGLEUF | ADC_IFC_SCANOF | ADC_IFC_SINGLEOF);
		/*Clear the FIFOs*/
		ADC0->SCANFIFOCLEAR = ADC_SCANFIFOCLEAR_SCANFIFOCLEAR;
		ADC0->SINGLEFIFOCLEAR = ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR;
		ADC0->CTRL |= ADC_CTRL_ADCCLKMODE_ASYNC;

}
void ADC_ASYNC_To_SYNC()
{
		NVIC_DisableIRQ(ADC0_IRQn);
		ADC_IntDisable(ADC0, ADC_IFC_PROGERR | ADC_IFC_VREFOV | ADC_IFC_SCANCMP | ADC_IFC_SINGLECMP | ADC_IFC_SCANUF | ADC_IFC_SINGLEUF | ADC_IFC_SCANOF | ADC_IFC_SINGLEOF);
		/*Clear the FIFOs*/
		ADC0->CTRL &= ~ADC_CTRL_ADCCLKMODE_ASYNC;
		ADC0->SCANFIFOCLEAR = ADC_SCANFIFOCLEAR_SCANFIFOCLEAR;
		ADC0->SINGLEFIFOCLEAR = ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR;

}

void ADC_Initialize()
{
	ADC_Init_TypeDef Init =
	{
			  .ovsRateSel = adcOvsRateSel2,
			  .prescale = OPERATING_FREQ/(NO_OF_SAMPLES + 1),
			  .warmUpMode = adcWarmupNormal,
	          .timebase = ADC_TimebaseCalc(0),
			  .tailgate = false,
			  .em2ClockConfig = adcEm2ClockOnDemand
	};

	ADC_InitSingle_TypeDef singleInit =
	{
			  .acqTime = adcAcqTime32,
			  .reference = adcRefVDD,
			  .resolution = adcRes12Bit,
			  .posSel = adcPosSelAPORT3XCH8,
			  .negSel=adcNegSelVSS,
			  .diff = false,
			  .prsEnable = false,
			  .leftAdjust = false ,
			  .rep = true,
			  .singleDmaEm2Wu = false,
			  .fifoOverwrite = true
	};

	ADC_Init(ADC0, &Init);
	ADC_InitSingle(ADC0, &singleInit);
	ADC0->SCANCTRL |= _ADC_SCANCTRL_REF_VDD;
	ADC0->SCANCTRLX |= _ADC_SCANCTRLX_VREFSEL_VDDXWATT;
	ADC0->CMPTHR = _ADC_CMPTHR_RESETVALUE;
	ADC0->CMPTHR = (adc_cmp_gt << _ADC_CMPTHR_ADGT_SHIFT) +
		 	                 (adc_cmp_lt << _ADC_CMPTHR_ADLT_SHIFT);
	ADC0->BIASPROG |=  ADC_BIASPROG_GPBIASACC | ADC_BIASPROG_ADCBIASPROG_SCALE32;
	ADC0->SINGLECTRL |=  ADC_SINGLECTRL_CMPEN;
	ADC_SYNC_To_ASYNC();
	ADC_IntClear(ADC0, ADC_IFC_PROGERR | ADC_IFC_VREFOV | ADC_IFC_SCANCMP | ADC_IFC_SINGLECMP | ADC_IFC_SCANUF | ADC_IFC_SINGLEUF | ADC_IFC_SCANOF | ADC_IFC_SINGLEOF);
	ADC_IntEnable(ADC0, ADC_IEN_SINGLECMP );
	blockSleepMode(energy_mode_3);
	CORE_ATOMIC_IRQ_ENABLE();
	NVIC_EnableIRQ(ADC0_IRQn);
}
