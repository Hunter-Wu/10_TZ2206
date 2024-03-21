#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "ddl.h"

void adc_init(void);
uint16_t get_adc_value(uint8_t channel);

#endif //__BSP_ADC_H

