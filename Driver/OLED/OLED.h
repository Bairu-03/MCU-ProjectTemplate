/*
 * OLED.h
 *
 *  Created on: 2024年7月14日
 *      Author: Bairu
 */

#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>

#define OLED_ADDRESS 0x3C   //OLED设备地址

#define OLED_WriteCom_Addr  0x00    //从机写指令地址
#define OLED_WriteData_Addr 0x40    //从机写数据地址

/********************滚屏参数宏定义********************/

#define Line1 0x00
#define Line2 0x01
#define Line3 0x02
#define Line4 0x03
#define Line5 0x04
#define Line6 0x05
#define Line7 0x06
#define Line8 0x07

#define ScrL 0x27
#define ScrR 0x26

/**********************函数声明************************/

void OLED_WriteCommand(uint8_t I2C_Command);
void OLED_WriteData(uint8_t IIC_Data);
void OLED_SetCursor(uint8_t Line, uint8_t Column);
void OLED_Display_Off(void);
void OLED_Display_On(void);
void OLED_Clear(void);
void OLED_Scroll(uint8_t LineS, uint8_t LineE, uint8_t ScrLR, uint8_t Level);
void OLED_Stop_Scroll(void);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);

void OLED_ShowChar(uint8_t Line, uint8_t Column, int8_t Char, uint8_t Size);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t Size);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num);
void OLED_DrawBMP(uint8_t LineS, uint8_t LineE, uint8_t ColumnS, uint8_t ColumnE, uint8_t BMP[]);

void OLED_Init(void);

#endif

