#include "driverlib.h"
#include "Sys_Clock.h"

/**
 * @brief  ��ʼ��ϵͳʱ�ӡ�
 *      XT1��ΪACLKʱ��Դ = 32768Hz��
 *      DCOCLK��ΪMCLKʱ��Դ = 25MHz��
 *      DCOCLK��ΪSMCLKʱ��Դ = 25MHz��
 *      ��������Ҫ��ȷʱ�ӿ��ƺ͸�Ƶ�����еĳ�����
 * @param  ��
 * @retval ��
 */
void SystemClock_Init(void)
{
    //��ߺ��ĵ�ѹ���������Ƶ��������
    PMM_setVCore(PMM_CORE_LEVEL_3);

    //����XT1���Ų�����XT1
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN5);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_3,UCS_XCAP_3);

    //����XT2���Ų�����XT2
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);

    //XT2��ΪFLL�ο�ʱ�ӣ���8��Ƶ����50��Ƶ 4MHz / 8 * 50 = 25MHz
    UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_8);
    UCS_initFLLSettle(25000, 50);

    //XT1��ΪACLKʱ��Դ = 32768Hz
    //DCOCLK��ΪMCLKʱ��Դ = 25MHz
    //DCOCLK��ΪSMCLKʱ��Դ = 25MHz
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    //�����ⲿʱ��Դ��Ƶ�ʣ�ʹ���ڵ���UCS_getMCLK, UCS_getSMCLK �� UCS_getACLKʱ�ɵõ���ȷֵ
    UCS_setExternalClockSource(32768, 4000000);
}
