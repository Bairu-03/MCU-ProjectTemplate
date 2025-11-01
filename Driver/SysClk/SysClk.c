#include "SysClk.h"

/**
  * @brief  初始化系统时钟.
  * @param  CLK_SYSCLKSource : 时钟源.
  *         参数取值参考枚举类型 CLK_SYSCLKSource_TypeDef
  * @param  CLK_SYSCLKDiv : 分频系数.
  *         参数取值参考枚举类型 CLK_SYSCLKDiv_TypeDef
  * @retval 无
  */
void SYSCLK_Config(CLK_SYSCLKSource_TypeDef CLK_SYSCLKSource, CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv)
{
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv);
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource)
    {
    }
}
