/*
 * MPU6050.c
 *
 *  Created on: 2022年7月17日
 *      Author: S10
 */
#include "driverlib.h"
#include "MPU6050.h"
#include "I2C_Sim.h"

#define  MPU_SCL_port  GPIO_PORT_P8
#define  MPU_SCL_pin   GPIO_PIN1
#define  MPU_SDA_port  GPIO_PORT_P2
#define  MPU_SDA_pin   GPIO_PIN3

/**
 * @brief  向MPU6050发送数据。
 * @param  REG_Address 内部寄存器地址。
 * @param  REG_data 待发送的字节。
 * @retval 无
 */
void MPU6050_WriteData(uint8_t REG_Address,uint8_t REG_data)
{
    I2C_Start(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin);                  //起始信号
    I2C_SendByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, REG_Address);    //内部寄存器地址
    I2C_SendByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, REG_data);       //向内部寄存器发送数据
    I2C_Stop(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin);                   //发送停止信号
}

/**
 * @brief  读取MPU6050寄存器数据。
 * @param  REG_Address 内部寄存器地址。
 * @retval 读取到的字节数据。
 */
uint8_t MPU6050_ReadData(uint8_t REG_Address)
{
    uint8_t REG_data;
    I2C_Start(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin);                   //起始信号
    I2C_SendByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, SlaveAddress);    //发送设备地址+写信号
    I2C_SendByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, REG_Address);     //发送存储单元地址，从0开始
    I2C_Start(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin);                   //起始信号
    I2C_SendByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, SlaveAddress+1);  //发送设备地址+读信号
    REG_data=I2C_ReadByte(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin);       //读出寄存器数据
    I2C_SendACK(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin, 1);                //接收应答信号
    I2C_Stop(MPU_SCL_port, MPU_SCL_pin, MPU_SDA_port, MPU_SDA_pin);                    //停止信号
    return REG_data;
}

/**
 * @brief  初始化MPU6050。
 *       P1.3 - SDA | P1.2 - SCL
 * @param  无
 * @retval 无
 */
void InitMPU6050(void)
{
    MPU6050_WriteData(PWR_MGMT_1, 0x00);  // 解除休眠状态
    MPU6050_WriteData(SMPLRT_DIV, 0x07);  // 陀螺仪采样率设置（125HZ）
    MPU6050_WriteData(MPU6050_CONFIG, 0x06);      // 低通滤波器设置（5HZ频率）
    MPU6050_WriteData(GYRO_CONFIG, 0x18); // 陀螺仪自检及检测范围设置(不自检,16.4LSB/DBS/S)
    MPU6050_WriteData(ACCEL_CONFIG, 0x01); // 不自检，量程2g
}

/*
 *************************************
 **合成数据
 *************************************
 */
int16_t Get6050Data(uint8_t REG_Address)
{
    char H, L;
    H = MPU6050_ReadData(REG_Address);
    L = MPU6050_ReadData(REG_Address + 1);
    return (H << 8) + L; // 合成数据
}

/*
**********************************************
**函数名  ：float Mpu6050AccelAngle(int8 dir)
**函数功能：输出加速度传感器测量的倾角值
**            范围为2g时，换算关系：16384 LSB/g
**            角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14
**            因为x>=sinx,故乘以1.2适当放大
**返回参数：测量的倾角值
**传入参数：dir - 需要测量的方向
**           ACCEL_XOUT - X方向
**           ACCEL_YOUT - Y方向
**           ACCEL_ZOUT - Z方向
**********************************************
*/
float Mpu6050AccelAngle(char dir)
{
    float accel_agle; // 测量的倾角值
    float result; // 测量值缓存变量
    result = (float)Get6050Data(dir); // 测量当前方向的加速度值,转换为浮点数
    accel_agle = (result + MPU6050_ZERO_ACCELL)/16384; // 去除零点偏移,计算得到角度（弧度）
    accel_agle = accel_agle*1*180/3.14;     //弧度转换为度

    return accel_agle; // 返回测量值
}

/*
**********************************************
**函数名  ：float Mpu6050GyroAngle(int8 dir)
**函数功能：输出陀螺仪测量的倾角加速度
**            范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)
**返回参数：测量的倾角加速度值
**传入参数：dir - 需要测量的方向
**           GYRO_XOUT - X轴方向
**           GYRO_YOUT - Y轴方向
**           GYRO_ZOUT - Z轴方向
**********************************************
*/
float Mpu6050GyroAngle(char dir)
{
    float gyro_angle;
    gyro_angle = (float)Get6050Data(dir);   // 检测陀螺仪的当前值
    gyro_angle = -(gyro_angle + MPU6050_ZERO_GYRO)/16.4;    //去除零点偏移，计算角速度值,负号为方向处理

    return gyro_angle; // 返回测量值
}


