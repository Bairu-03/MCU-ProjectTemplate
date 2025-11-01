#include "ADC.h"

// ADC初始化配置
// 启用 通道1 - PA5 和 通道2 - PA4
void ADC_Config(void)
{
    GPIO_Init(ADC_PORT, ADC_CH1_PIN | ADC_CH2_PIN, GPIO_Mode_In_FL_No_IT); // 初始化GPIO

    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE); // 使能ADC外设时钟
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_Cmd(ADC1, ENABLE);
}

// 读取ADC通道1电压值 - PA5
float ADC_GetCH1Voltage(void)
{
    uint16_t ADCdata = 0;

    ADC_ChannelCmd(ADC1, ADC_Channel_2, DISABLE);
    ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);

    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {
    }
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    ADCdata = ADC_GetConversionValue(ADC1);

    return (float)((float)ADCdata / ADC_MaxSamplingValue * ADC_VREF);
}

// 读取ADC通道2电压值 - PA4
float ADC_GetCH2Voltage(void)
{
    uint16_t ADCdata = 0;

    ADC_ChannelCmd(ADC1, ADC_Channel_1, DISABLE);
    ADC_ChannelCmd(ADC1, ADC_Channel_2, ENABLE);

    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {
    }
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    ADCdata = ADC_GetConversionValue(ADC1);

    return (float)((float)ADCdata / ADC_MaxSamplingValue * ADC_VREF);
}
