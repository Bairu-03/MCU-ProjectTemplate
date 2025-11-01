#include "KEY.h"

// 按键状态变量, 主程序中读取后需要手动重置为 Key_NotPress 状态
volatile Key_PressStatus_TypeDef g_KeyPressStatus = Key_NotPress;

// 按键B2初始化
void Key_Config(void)
{
    GPIO_Init(KEY_B2_PORT, KEY_B2_PIN, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
}
