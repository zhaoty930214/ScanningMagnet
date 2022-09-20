/**
 ****************************************************************************************************
 * @file        pid.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       PID�㷨����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "pid.h"
#include "stepper_motor.h"
#include <stdlib.h>

PID_TypeDef  g_location_pid;                    /* λ��PID�����ṹ��*/

/**
 * @brief       ��ʼ��PID����
 * @param       ��
 * @retval      ��
 */
void pid_init(void)
{
    /*λ�û���ʼ��*/
    g_location_pid.SetPoint = 0 /*(float)(50*PPM)*/;  /* �趨Ŀ��Desired Value*/
    g_location_pid.ActualValue = 0.0;           /* ����ֵ*/
    g_location_pid.SumError = 0.0;              /* ����ֵ*/
    g_location_pid.Error = 0.0;                 /* Error[1]*/
    g_location_pid.LastError = 0.0;             /* Error[-1]*/
    g_location_pid.PrevError = 0.0;             /* Error[-2]*/
    g_location_pid.Proportion = L_KP;           /* �������� Proportional Const*/
    g_location_pid.Integral =   L_KI;             /* ���ֳ��� Integral Const*/
    g_location_pid.Derivative = L_KD;           /* ΢�ֳ��� Derivative Const*/
    g_location_pid.IngMax = 20;
    g_location_pid.IngMin = -20;
    g_location_pid.OutMax = 50;                /* ������� */
    g_location_pid.OutMin = -50;
}


/**
  * �������ƣ�λ�ñջ�PID�������
  * �����������ǰ������
  * �� �� ֵ��Ŀ�������
  * ˵    ������
  */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value)
{
     PID->Error = (float)(PID->SetPoint - Feedback_value);                  /* ƫ�� */
#if INCR_LOCT_SELECT
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))   /* E[k]�� */
                        + (PID->Integral * PID->Error)                      /* E[k-1]�� */
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError)); /* E[k-2]�� */
    PID->PrevError = PID->LastError;                                        /* �洢�������´μ��� */
    PID->LastError = PID->Error;
#else
    PID->SumError += PID->Error;
    if(PID->SumError > PID->IngMax)
    {
        PID->SumError = PID->IngMax;
    }
    else if(PID->SumError < PID->IngMin)
    {
        PID->SumError = PID->IngMin;
    }
    
    PID->ActualValue = (PID->Proportion * PID->Error)                       /* E[k]�� */
                       + (PID->Integral * PID->SumError)                    /* E[k-1]�� */
                       + (PID->Derivative * (PID->Error - PID->LastError)); /* E[k-2]�� */
    PID->LastError = PID->Error;
#endif
    if(PID->ActualValue > PID->OutMax)
    {
        PID->ActualValue = PID->OutMax;
    }
    else if(PID->ActualValue < PID->OutMin)
    {
        PID->ActualValue = PID->OutMin;
    }

//    if( abs(PID->ActualValue) < 10)
//    {
//    	PID->ActualValue = 0;
//    }
    return ((int32_t)(PID->ActualValue));                                   /* ����ʵ�ʿ�����ֵ */

}
