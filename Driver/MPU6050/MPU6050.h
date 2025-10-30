/*
 * MPU6050.h
 *
 *  Created on: 2022年7月17日
 *      Author: S10
 */

#ifndef MPU6050_H_
#define MPU6050_H_

/******************MPU6050地址******************/
#define SlaveAddress    0xD0    // A0接地，若接3.3v则0xD1
/**************MPU6050的零点校准值***************/
#define MPU6050_ZERO_ACCELL 378
#define MPU6050_ZERO_GYRO 13
/**************定义MPU6050内部地址***************/
#define SMPLRT_DIV      0x19    //输出8位无符号位，输出分频，用来配置采样频率的寄存器。采样率=陀螺仪的输出率/（1+该寄存器值),低通滤波器使能时陀螺仪输出为8k，反之1k。
#define MPU6050_CONFIG          0x1A    //配置低通滤波器的寄存器
#define GYRO_CONFIG     0x1B    //三个方向角度的自我测试和量程
#define ACCEL_CONFIG    0x1C    //三个方向加速度的自我测试和量程
/****************加速度传感器寄存器***************/
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_XOUT      ACCEL_XOUT_H    // X轴读取地址，高位为起始位
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_YOUT      ACCEL_YOUT_H    // Y轴读取地址，高位为起始位
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define ACCEL_ZOUT      ACCEL_ZOUT_H    // Z轴读取地址，高位为起始位
/*****************温度传感器寄存器****************/
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define TEMP_OUT          TEMP_OUT_H    // 温度传感器读取地址，高位为起始位
/*****************陀螺仪相关寄存器****************/
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_XOUT        GYRO_XOUT_H    // 陀螺仪X轴读取地址，高位为起始位
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_YOUT        GYRO_YOUT_H    // 陀螺仪Y轴读取地址，高位为起始位
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define GYRO_ZOUT        GYRO_ZOUT_H    // 陀螺仪Z轴读取地址，高位为起始位
/*******************其它寄存器********************/
#define PWR_MGMT_1      0x6B    // 电源管理
#define WHO_AM_I        0x75    // 6位iic地址，最后一位A0控制

void MPU6050_WriteData(uint8_t REG_Address,uint8_t REG_data);
uint8_t MPU6050_ReadData(uint8_t REG_Address);

void InitMPU6050(void);
int16_t Get6050Data(uint8_t REG_Address);
float Mpu6050AccelAngle(char dir);
float Mpu6050GyroAngle(char dir);

#endif /* MPU6050_H_ */
