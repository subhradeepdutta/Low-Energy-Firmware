
#ifndef INC_ADC_H_
#define INC_ADC_H_


#include "em_adc.h"
#include "em_core.h"
#include "gpio.h"
#include "sleep.h"
#include <stdbool.h>

#define OPERATING_FREQ (1000)
#define NO_OF_SAMPLES (200)

void ADC_Initialize();

#endif /* INC_ADC_H_ */

