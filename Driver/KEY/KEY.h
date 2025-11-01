#ifndef __KEY_H
#define __KEY_H

#include "stm8l15x.h"
#include "stdbool.h"

#define KEY_B2_PORT     GPIOC
#define KEY_B2_PIN      GPIO_Pin_1

#define IS_KEY2_PRESSED() (GPIO_ReadInputDataBit(KEY_B2_PORT, KEY_B2_PIN) == RESET)

// 按键状态
typedef enum
{
    Key_NotPress,   // 按键未按动
    Key_ShortPress, // 按键短按
    Key_LongPress,  // 按键长按
} Key_PressStatus_TypeDef;

extern volatile Key_PressStatus_TypeDef g_KeyPressStatus;

void Key_Config(void);

#endif /* __KEY_H */
