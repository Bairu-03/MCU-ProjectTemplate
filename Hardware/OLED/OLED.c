/*
 * OLED.c
 *
 *  Created on: 2024��7��14��
 *      Author: Bairu
 */

#include "MSP430F5529_I2C.h"
#include "OLED.h"
#include "OLED_Font.h"

uint16_t TxData = 0;

/**
 * @brief  ��OLED���������
 * @param  I2C_Command Ҫд������
 * @retval ��
 */
void OLED_WriteCommand(uint8_t I2C_Command)
{
    TxData = (OLED_WriteCom_Addr << 8) | I2C_Command;
    writeWord(TxData);
}

/**
 * @brief  ��OLED���������ݡ�
 * @param  IIC_Data Ҫд������ݡ�
 * @retval ��
 */
void OLED_WriteData(uint8_t IIC_Data)
{
    TxData = (OLED_WriteData_Addr << 8) | IIC_Data;
    writeWord(TxData);
}

/**
 * @brief  ������Ļ��ʾ��ʼ���ꡣ
 * @param  Line �У�ҳ����ַ�������Ͻ�Ϊԭ�㣬���·�������ꡣ
 *     @arg ȡֵ: 0 - 7
 * @param  Column �е�ַ�������Ͻ�Ϊԭ�㣬���ҷ�������ꡣ
 *     @arg ȡֵ: 0 - 127
 * @retval ��
 */
void OLED_SetCursor(uint8_t Line, uint8_t Column)
{
    OLED_WriteCommand(0xB0 | Line);
    OLED_WriteCommand(0x10 | ((Column & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (Column & 0x0F));
}

/**
 * @brief  OLEDϨ����
 * @param  ��
 * @retval ��
 */
void OLED_Display_Off(void)
{
    OLED_WriteCommand(0XAE); // Ϩ��
    OLED_WriteCommand(0X8D); // ����OLED��ɱ�
    OLED_WriteCommand(0X10); // ʧ�ܣ���
}

/**
 * @brief  OLED������
 * @param  ��
 * @retval ��
 */
void OLED_Display_On(void)
{
    OLED_WriteCommand(0X8D); // ����OLED��ɱ�
    OLED_WriteCommand(0X14); // ʹ�ܣ���
    OLED_WriteCommand(0XAF); // ����
}

/**
 * @brief  OLED������
 * @param  ��
 * @retval ��
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
 * @brief  OLEDָ����ˮƽ������
 *      ע�⣺�����Ƚ���ʾ���ݴ�����ɺ����������������������롣
 *      �Ƽ�˳�򣺵���OLED_Stop_Scroll -> ����OLED_Clear -> ������ʾ���� -> ����OLED_Scroll
 * @param  LineS �����з�Χ: ��һ���кš�
 *     @arg ��Чȡֵ:
 *      - \b Line1
 *      - \b Line2
 *      - \b Line3
 *      - \b Line4
 *      - \b Line5
 *      - \b Line6
 *      - \b Line7
 *      - \b Line8
 * @param  LineE �����з�Χ: ���һ���кš�
 *     @arg ��Чȡֵ:
 *      - \b Line1
 *      - \b Line2
 *      - \b Line3
 *      - \b Line4
 *      - \b Line5
 *      - \b Line6
 *      - \b Line7
 *      - \b Line8
 * @param  ScrLR ��������
 *     @arg ��Чȡֵ:
 *      - \b ScrL : �������
 *      - \b ScrR : ���ҹ���
 * @param  Level �����ٶȵȼ���
 *     @arg ȡֵ: 0 - 7���� - �죩
 * @retval ��
 */
void OLED_Scroll(uint8_t LineS, uint8_t LineE, uint8_t ScrLR, uint8_t Level)
{
    // SSD1306�ֲ��й涨�Ĳ�ͬ�����ٶ�ָ��������֡����һ�Σ�
    uint8_t Speed[8] = {
        0x03,   // 256֡
        0x02,   // 128֡
        0x01,   // 64֡
        0x06,   // 25֡
        0x00,   // 5֡
        0x05,   // 4֡
        0x04,   // 3֡
        0x07    // 2֡
    };

    OLED_WriteCommand(0x2E);  // �رչ���
    OLED_WriteCommand(ScrLR); // ˮƽ�������
    OLED_WriteCommand(0x00);  // �����ֽ�
    OLED_WriteCommand(LineS); // ��ʼ��
    OLED_WriteCommand(Speed[Level]); // �����ٶȣ�֡��
    OLED_WriteCommand(LineE); // ��ֹ��
    OLED_WriteCommand(0x00);  // �����ֽ�
    OLED_WriteCommand(0xFF);  // �����ֽ�
    OLED_WriteCommand(0x2F);  // ��������
}

/**
 * @brief  OLEDֹͣ��Ļ������
 * @param  ��
 * @retval ��
 */
void OLED_Stop_Scroll(void)
{
    OLED_WriteCommand(0x2E); // �رչ���
}

/**
 * @brief  ����x^y��
 * @param  X �޷�����������
 * @param  Y �޷�����������
 * @retval Result X��Y�η�
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
 * @brief  OLED��ʾһ���ַ���
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Char Ҫ��ʾ��һ���ַ���
 *     @arg ȡֵ: ASCII�ɼ��ַ�
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, int8_t Char, uint8_t Size)
{
    uint8_t i;
    if (Size == 8) // �ַ���С8x16
    {
        OLED_SetCursor(Line - 1, Column - 1); // ���ù��λ�����ϰ벿��
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_F8x16[Char - ' '][i]); // ��ʾ�ϰ벿������
        }
        OLED_SetCursor((Line - 1) + 1, Column - 1); // ���ù��λ�����°벿��
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]); // ��ʾ�°벿������
        }
    }
    else // �ַ���С6x8
    {
        OLED_SetCursor(Line - 1, Column - 1);
        for (i = 0; i < 6; i++)
        {
            OLED_WriteData(OLED_F6x8[Char - ' '][i]);
        }
    }
}

/**
 * @brief  OLED��ʾ�ַ�����
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  String Ҫ��ʾ���ַ�����
 *     @arg ȡֵ: ASCII�ɼ��ַ�
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
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
 * @brief  OLED��ʾ���֣�ʮ���ƣ���������
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Number Ҫ��ʾ�����֡�
 *     @arg ȡֵ: 0 - 4294967295
 * @param  Length Ҫ��ʾ���ֵĳ��ȡ�
 *     @arg ȡֵ: 1 - 10
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
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
 * @brief  OLED��ʾʮ���ƴ���������
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Number Ҫ��ʾ�����֡�
 *     @arg ȡֵ: -2147483648 - +2147483647
 * @param  Length Ҫ��ʾ���ֵĳ��ȡ�
 *     @arg ȡֵ: 1 - 10
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
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
 * @brief  OLED��ʾ�з��Ÿ�����
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Num Ҫ��ʾ�����֡�
 *     @arg ȡֵ: -3.4028235E38 - +3.4028235E38
 * @param  Intlen Ҫ��ʾ������λ����
 *     @arg ȡֵ: 1 - 10
 * @param  Declen Ҫ��ʾ��С��λ����
 *     @arg ȡֵ: 1 - 10
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
 */
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size)
{
    uint8_t p;
    unsigned long m;

    OLED_ShowSignedNum(Line, Column, (int32_t)Num, Intlen, Size);

    if (Declen > 0)
        OLED_ShowString(Line, Column + Size * (Intlen + 1), ".", Size);

    if(Num < 0) Num = -Num;
    for (p = 2, m = 10; p <= Declen + 1; p++, m *= 10)
    {
        OLED_ShowNum(Line, Column + Size * (Intlen + p), (unsigned long)((Num - (uint32_t)Num) * m) % 10, 1, Size);
    }
}

/**
 * @brief  OLED��ʾ���֣�ʮ�����ƣ���������
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Number Ҫ��ʾ�����֡�
 *     @arg ȡֵ: 0 - 0xFFFFFFFF
 * @param  Length Ҫ��ʾ���ֵĳ��ȡ�
 *     @arg ȡֵ: 1 - 8
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
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
 * @brief  OLED��ʾ���֣������ƣ���������
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Number Ҫ��ʾ�����֡�
 *     @arg ȡֵ: 0 - 1111 1111 1111 1111
 * @param  Length Ҫ��ʾ���ֵĳ��ȡ�
 *     @arg ȡֵ: 1 - 16
 * @param  Size �ַ���С��
 *     @arg ȡֵ(��x��): 6��6x8����8��8x16��
 * @retval ��
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
 * @brief  OLED��ʾ���֡�
 * @param  Line ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  Column ��ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  Num Ҫ��ʾ�ĺ������ֿ������е������ţ��±�ţ���
 * @retval ��
 */
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num)
{
    uint8_t i;
    uint8_t wide = 16; // �ֿ�

    OLED_SetCursor(Line - 1, Column - 1); // ����1:�ѹ�������ڵڼ���. ����2:�ѹ�������ڵڼ���
    for (i = 0; i < wide; i++)
    {
        OLED_WriteData(OLED_HzK[Num][i]); // ��ʾ�ϰ벿������
    }
    OLED_SetCursor((Line - 1) + 1, Column - 1);
    for (i = 0; i < wide; i++)
    {
        OLED_WriteData(OLED_HzK[Num][i + wide]); // ��ʾ�°벿������
    }
}

/**
 * @brief  OLED��ʾͼƬ��
 * @param  LineS ����ͼƬ����ʼ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  LineE ����ͼƬ����ֹ��λ�á�
 *     @arg ȡֵ: 1 - 8
 * @param  ColumnS ����ͼƬ����ʼ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  ColumnE ����ͼƬ����ֹ��λ�á�
 *     @arg ȡֵ: 1 - 128
 * @param  BMP ͼƬģ���顣
 * @retval ��
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
 * @brief  OLED��ʼ��
 *          \n P3.0 - SDA
 *          \n P3.1 - SCL
 * @param  ��
 * @retval ��
 */
void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0; i < 1000; i++) // �ϵ���ʱ
    {
        for (j = 0; j < 1000; j++)
            ;
    }

    // ��ʼ��I2C
    Init_I2C_GPIO();
    I2C_init(OLED_ADDRESS >> 1);

    OLED_WriteCommand(0xAE); // �ر���ʾ

    OLED_WriteCommand(0xD5); // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // ���ö�·������
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // ������ʾƫ��
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // ������ʾ��ʼ��

    OLED_WriteCommand(0xA1); // �������ҷ���0xA1���� 0xA0���ҷ���

    OLED_WriteCommand(0xC8); // �������·���0xC8���� 0xC0���·���

    OLED_WriteCommand(0xDA); // ����COM����Ӳ������
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // ���öԱȶȿ���
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9); // ����Ԥ�������
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // ����VCOMHȡ��ѡ�񼶱�
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // ����������ʾ��/�ر�

    OLED_WriteCommand(0xA6); // ��������/��ת��ʾ

    OLED_WriteCommand(0x8D); // ���ó���
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // ������ʾ

    OLED_Clear(); // OLED����
}

