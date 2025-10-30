/*
 * OLED.c
 *
 *  Created on: 2024年7月14日
 *      Author: Bairu
 */

#include "MSP430F5529_I2C.h"
#include "OLED.h"
#include "OLED_Font.h"


/*************************************硬件I2C****************************************/
/***********************"../I2C_Hardware/MSP430F5529_I2C.h"**************************/
/**
 * @brief  向OLED屏发送命令（硬件I2C）。
 * @param  I2C_Command 要写入的命令。
 * @retval 无
 */
void OLED_WriteCommand(uint8_t I2C_Command)
{
    uint16_t TxData = 0;
    TxData = (OLED_WriteCom_Addr << 8) | I2C_Command;
    writeWord(TxData);
}

/**
 * @brief  向OLED屏发送数据（硬件I2C）。
 * @param  IIC_Data 要写入的数据。
 * @retval 无
 */
void OLED_WriteData(uint8_t IIC_Data)
{
    uint16_t TxData = 0;
    TxData = (OLED_WriteData_Addr << 8) | IIC_Data;
    writeWord(TxData);
}
/************************************************************************************/


/*********************************软件（模拟I2C）*************************************/
/**************************"../I2C_Software/I2C_Sim.h"*******************************/
// // 定义软件I2C引脚
// #define OLED_SCL_port GPIO_PORT_P2
// #define OLED_SCL_pin  GPIO_PIN4
// #define OLED_SDA_port GPIO_PORT_P1
// #define OLED_SDA_pin  GPIO_PIN5

// /**
//  * @brief  向OLED屏发送命令（软件I2C）。
//  * @param  I2C_Command 要写入的命令。
//  * @retval 无
//  */
// void OLED_WriteCommand(uint8_t I2C_Command)
// {
//     I2C_Start(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin);
//     I2C_SendByte(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin, 0x78);
//     I2C_SendByte(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin, 0x00);
//     I2C_SendByte(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin, I2C_Command);
//     I2C_Stop(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin);
// }

// /**
//  * @brief  向OLED屏发送数据（软件I2C）。
//  * @param  IIC_Data 要写入的数据。
//  * @retval 无
//  */
// void OLED_WriteData(uint8_t IIC_Data)
// {
//     I2C_Start(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin);
//     I2C_SendByte(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin, 0x78);
//     I2C_SendByte(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin, 0x40);
//     I2C_SendByte(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin, IIC_Data);
//     I2C_Stop(OLED_SCL_port, OLED_SCL_pin, OLED_SDA_port, OLED_SDA_pin);
// }
/*************************************************************************************/


/**
 * @brief  设置屏幕显示起始坐标。
 * @param  Line 行（页）地址，以左上角为原点，向下方向的坐标。
 *     @arg 取值: 0 - 7
 * @param  Column 列地址，以左上角为原点，向右方向的坐标。
 *     @arg 取值: 0 - 127
 * @retval 无
 */
void OLED_SetCursor(uint8_t Line, uint8_t Column)
{
    OLED_WriteCommand(0xB0 | Line);
    OLED_WriteCommand(0x10 | ((Column & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (Column & 0x0F));
}

/**
 * @brief  OLED熄屏。
 * @param  无
 * @retval 无
 */
void OLED_Display_Off(void)
{
    OLED_WriteCommand(0XAE); // 熄屏
    OLED_WriteCommand(0X8D); // 设置OLED电荷泵
    OLED_WriteCommand(0X10); // 失能，关
}

/**
 * @brief  OLED亮屏。
 * @param  无
 * @retval 无
 */
void OLED_Display_On(void)
{
    OLED_WriteCommand(0X8D); // 设置OLED电荷泵
    OLED_WriteCommand(0X14); // 使能，开
    OLED_WriteCommand(0XAF); // 亮屏
}

/**
 * @brief  OLED清屏。
 * @param  无
 * @retval 无
 */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/**
 * @brief  OLED指定行水平滚动。
 *      注意：必须先将显示数据传输完成后再启动滚动，否则极易乱码。
 *      推荐顺序：调用OLED_Stop_Scroll -> 调用OLED_Clear -> 传输显示数据 -> 调用OLED_Scroll
 * @param  LineS 滚动行范围: 第一行行号。
 *     @arg 有效取值:
 *      - \b Line1
 *      - \b Line2
 *      - \b Line3
 *      - \b Line4
 *      - \b Line5
 *      - \b Line6
 *      - \b Line7
 *      - \b Line8
 * @param  LineE 滚动行范围: 最后一行行号。
 *     @arg 有效取值:
 *      - \b Line1
 *      - \b Line2
 *      - \b Line3
 *      - \b Line4
 *      - \b Line5
 *      - \b Line6
 *      - \b Line7
 *      - \b Line8
 * @param  ScrLR 滚动方向。
 *     @arg 有效取值:
 *      - \b ScrL : 向左滚动
 *      - \b ScrR : 向右滚动
 * @param  Level 滚动速度等级。
 *     @arg 取值: 0 - 7（慢 - 快）
 * @retval 无
 */
void OLED_Scroll(uint8_t LineS, uint8_t LineE, uint8_t ScrLR, uint8_t Level)
{
    // SSD1306手册中规定的不同滚动速度指令（间隔多少帧滚动一次）
    uint8_t Speed[8] = {
        0x03,   // 256帧
        0x02,   // 128帧
        0x01,   // 64帧
        0x06,   // 25帧
        0x00,   // 5帧
        0x05,   // 4帧
        0x04,   // 3帧
        0x07    // 2帧
    };

    OLED_WriteCommand(0x2E);  // 关闭滚动
    OLED_WriteCommand(ScrLR); // 水平向左滚动
    OLED_WriteCommand(0x00);  // 虚拟字节
    OLED_WriteCommand(LineS); // 起始行
    OLED_WriteCommand(Speed[Level]); // 滚动速度（帧）
    OLED_WriteCommand(LineE); // 终止行
    OLED_WriteCommand(0x00);  // 虚拟字节
    OLED_WriteCommand(0xFF);  // 虚拟字节
    OLED_WriteCommand(0x2F);  // 开启滚动
}

/**
 * @brief  OLED停止屏幕滚动。
 * @param  无
 * @retval 无
 */
void OLED_Stop_Scroll(void)
{
    OLED_WriteCommand(0x2E); // 关闭滚动
}

/**
 * @brief  计算x^y。
 * @param  X 无符号整形数。
 * @param  Y 无符号整形数。
 * @retval Result X的Y次方
 */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  OLED显示一个字符。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Char 要显示的一个字符。
 *     @arg 取值: ASCII可见字符
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, int8_t Char, uint8_t Size)
{
    uint8_t i;
    if (Size == 8) // 字符大小8x16
    {
        OLED_SetCursor(Line - 1, Column - 1); // 设置光标位置在上半部分
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_F8x16[Char - ' '][i]); // 显示上半部分内容
        }
        OLED_SetCursor((Line - 1) + 1, Column - 1); // 设置光标位置在下半部分
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]); // 显示下半部分内容
        }
    }
    else // 字符大小6x8
    {
        OLED_SetCursor(Line - 1, Column - 1);
        for (i = 0; i < 6; i++)
        {
            OLED_WriteData(OLED_F6x8[Char - ' '][i]);
        }
    }
}

/**
 * @brief  OLED显示字符串。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  String 要显示的字符串。
 *     @arg 取值: ASCII可见字符
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t Size)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + Size * i, String[i], Size);
    }
}

/**
 * @brief  OLED显示数字（十进制，正数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: 0 - 4294967295
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 10
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + Size * i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', Size);
    }
}

/**
 * @brief  OLED显示十进制带符号数。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: -2147483648 - +2147483647
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 10
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint8_t Size)
{
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+', Size);
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-', Size);
        Number1 = -Number;
    }
    OLED_ShowNum(Line, Column + Size, Number1, Length, Size);
}

/**
 * @brief  OLED显示有符号浮点数
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Num 要显示的数字。
 *     @arg 取值: -3.4028235E38 - +3.4028235E38
 * @param  Intlen 要显示的整数位数。
 *     @arg 取值: 1 - 10
 * @param  Declen 要显示的小数位数。
 *     @arg 取值: 1 - 10
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size)
{
    uint8_t p;
    unsigned long m;

    OLED_ShowSignedNum(Line, Column, (int32_t)Num, Intlen, Size);

    if (Declen > 0)
        OLED_ShowChar(Line, Column + Size * (Intlen + 1), '.', Size);

    if(Num < 0) Num = -Num;
    for (p = 2, m = 10; p <= Declen + 1; p++, m *= 10)
    {
        OLED_ShowNum(Line, Column + Size * (Intlen + p), (unsigned long)((Num - (uint32_t)Num) * m) % 10, 1, Size);
    }
}

/**
 * @brief  OLED显示数字（十六进制，正数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: 0 - 0xFFFFFFFF
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 8
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + Size * i, SingleNumber + '0', Size);
        }
        else
        {
            OLED_ShowChar(Line, Column + Size * i, SingleNumber - 10 + 'A', Size);
        }
    }
}

/**
 * @brief  OLED显示数字（二进制，正数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: 0 - 1111 1111 1111 1111
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 16
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + Size * i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', Size);
    }
}

/**
 * @brief  OLED显示汉字。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Num 要显示的汉字在字库数组中的索引号（下标号）。
 * @retval 无
 */
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num)
{
    uint8_t i;
    uint8_t wide = 16; // 字宽

    OLED_SetCursor(Line - 1, Column - 1); // 参数1:把光标设置在第几行. 参数2:把光标设置在第几列
    for (i = 0; i < wide; i++)
    {
        OLED_WriteData(OLED_HzK[Num][i]); // 显示上半部分内容
    }
    OLED_SetCursor((Line - 1) + 1, Column - 1);
    for (i = 0; i < wide; i++)
    {
        OLED_WriteData(OLED_HzK[Num][i + wide]); // 显示下半部分内容
    }
}

/**
 * @brief  OLED显示图片。
 * @param  LineS 绘制图片的起始行位置。
 *     @arg 取值: 1 - 8
 * @param  LineE 绘制图片的终止行位置。
 *     @arg 取值: 1 - 8
 * @param  ColumnS 绘制图片的起始列位置。
 *     @arg 取值: 1 - 128
 * @param  ColumnE 绘制图片的终止列位置。
 *     @arg 取值: 1 - 128
 * @param  BMP 图片模数组。
 * @retval 无
 */
void OLED_DrawBMP(uint8_t LineS, uint8_t LineE, uint8_t ColumnS, uint8_t ColumnE, uint8_t BMP[])
{
    uint32_t j = 0;
    uint8_t x, y;

    if ((LineE - 1) % 8 == 0)
        y = (LineE - 1) / 8;
    else
        y = (LineE - 1) / 8 + 1;
    for (y = (LineS - 1); y <= (LineE - 1); y++)
    {
        OLED_SetCursor(y, (ColumnS - 1));
        for (x = (ColumnS - 1); x <= (ColumnE - 1); x++)
        {
            OLED_WriteData(BMP[j++]);
        }
    }
}

/**
 * @brief  OLED初始化。
 *      P3.0 - SDA | P3.1 - SCL
 * @param  无
 * @retval 无
 */
void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0; i < 1000; i++) // 上电延时
    {
        for (j = 0; j < 1000; j++)
            ;
    }

    I2C_init(OLED_ADDRESS); // 硬件I2C初始化（使用软件I2C时删除此函数）

    OLED_WriteCommand(0xAE); // 关闭显示

    OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // 设置多路复用率
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // 设置显示开始行

    OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // 设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6); // 设置正常/倒转显示

    OLED_WriteCommand(0x8D); // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear(); // OLED清屏
}

