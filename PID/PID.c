/*
 * PID.c
 *
 *  Created on: 2024��7��17��
 *      Author: Bairu
 */

#include "PID.h"

/**
 * @brief  ָ����Ȩ�ƶ�ƽ���˲�
 * @param  input ����ֵ
 * @param  filtered_value �ϴ��˲��������ֵ
 * @param  alpha �˲�ϵ��
 * @retval �˲����
 */
float exponential_filter(float input, float filtered_value, float alpha)
{
    return alpha * input + (1.0 - alpha) * filtered_value;
}

/**
 * @brief  PID������ʼ��
 * @param  *pid PID�����ṹ�����
 * @param  Kp ������ϵ��
 * @param  Ki ������ϵ��
 * @param  Kd ΢����ϵ��
 * @param  target Ŀ��ֵ
 * @param  minIntegral �����޷�-��Сֵ
 * @param  maxIntegral �����޷�-���ֵ
 * @param  minOutput PID����޷�-��Сֵ
 * @param  maxOutput PID����޷�-���ֵ
 * @retval ��
 */
void PID_Init(PID *pid, float Kp, float Ki, float Kd, float target,
              float minIntegral, float maxIntegral,
              float minOutput, float maxOutput)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->target = target;
    pid->minIntegral = minIntegral;
    pid->maxIntegral = maxIntegral;
    pid->minOutput = minOutput;
    pid->maxOutput = maxOutput;
    pid->last_error = 0.0;
    pid->integral = 0.0;
    pid->filtered_input = 0.0;
}

/**
 * @brief  ����PIDĿ��ֵ
 * @param  *pid PID�����ṹ�����
 * @param  target Ŀ��ֵ
 * @retval ��
 */
void PID_ResetTarget(PID *pid, float target)
{
    pid->target = target;
}

/**
 * @brief  λ��ʽPID���㡣
 *          ������ֵ��Ŀ��ֵ���ڲ��ʱ��PID������������仯ֱ���ﵽ�޷�ֵ��
 *          ������ֵ��Ŀ��ֵһ�º�PID���������ȶ���ĳһֵ��
 * @param  *pid PID�����ṹ�����
 * @param  input ��⵽��ʵʱֵ
 * @retval ��
 */
float PID_Compute(PID *pid, float input)
{
    // ������ֵ�����˲�
    pid->filtered_input = exponential_filter(input, pid->filtered_input, 0.2); // 0.2Ϊ�˲�ϵ��

    float error = pid->target - pid->filtered_input;    // �������

    float p_term = pid->Kp * error;    // ������
    float i_term = pid->integral + pid->Ki * error;    // ������
    float d_term = pid->Kd * (error - pid->last_error);    // ΢����

    // �����޷�
    if(i_term > pid->maxIntegral) i_term = pid->maxIntegral;
    else if(i_term < pid->minIntegral) i_term = pid->minIntegral;

    // ���»���ֵ���ϴ����
    pid->integral = i_term;
    pid->last_error = error;

    float PID_Output = p_term + i_term + d_term;

    // PID����޷�
    if(PID_Output > pid->maxOutput) PID_Output = pid->maxOutput;
    else if(PID_Output < pid->minOutput) PID_Output = pid->minOutput;

    return PID_Output;
}
