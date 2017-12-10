#include "ADC.h"

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
			  .reference = adcRef5V,/*Uses internal 5V reference*/
			  .resolution = adcRes12Bit,
			  .posSel = adcPosSelAPORT1XCH22,
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
	ADC0->BIASPROG |=  ADC_BIASPROG_GPBIASACC | ADC_BIASPROG_ADCBIASPROG_SCALE32;
	ADC_IntClear(ADC0, ADC_IFC_PROGERR | ADC_IFC_VREFOV | ADC_IFC_SCANCMP | ADC_IFC_SINGLECMP | ADC_IFC_SCANUF | ADC_IFC_SINGLEUF | ADC_IFC_SCANOF | ADC_IFC_SINGLEOF);
	NVIC_DisableIRQ(ADC0_IRQn);
}
