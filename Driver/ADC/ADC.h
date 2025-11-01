#ifndef __ADC_H
#define __ADC_H

#include "stm8l15x.h"

#define ADC_VREF ((float)3.3)
#define ADC_MaxSamplingValue ((float)4095)

#define ADC_PORT GPIOA
#define ADC_CH1_PIN GPIO_Pin_5  // 接GND
#define ADC_CH2_PIN GPIO_Pin_4  // 接3.3

void ADC_Config(void);
float ADC_GetCH1Voltage(void);
float ADC_GetCH2Voltage(void);

#endif /* __ADC_H */
