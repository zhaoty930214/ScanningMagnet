/**
 ****************************************************************************************************
 * @file        pid.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       PID算法代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "pid.h"
#include "stepper_motor.h"
#include <stdlib.h>

PID_TypeDef  g_location_pid;                    /* 位置PID参数结构体*/

/**
 * @brief       初始化PID参数
 * @param       无
 * @retval      无
 */
void pid_init(void)
{
    /*位置环初始化*/
    g_location_pid.SetPoint = 0 /*(float)(50*PPM)*/;  /* 设定目标Desired Value*/
    g_location_pid.ActualValue = 0.0;           /* 期望值*/
    g_location_pid.SumError = 0.0;              /* 积分值*/
    g_location_pid.Error = 0.0;                 /* Error[1]*/
    g_location_pid.LastError = 0.0;             /* Error[-1]*/
    g_location_pid.PrevError = 0.0;             /* Error[-2]*/
    g_location_pid.Proportion = L_KP;           /* 比例常数 Proportional Const*/
    g_location_pid.Integral =   L_KI;             /* 积分常数 Integral Const*/
    g_location_pid.Derivative = L_KD;           /* 微分常数 Derivative Const*/
    g_location_pid.IngMax = 20;
    g_location_pid.IngMin = -20;
    g_location_pid.OutMax = 50;                /* 输出限制 */
    g_location_pid.OutMin = -50;
}


/**
  * 函数名称：位置闭环PID控制设计
  * 输入参数：当前控制量
  * 返 回 值：目标控制量
  * 说    明：无
  */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value)
{
     PID->Error = (float)(PID->SetPoint - Feedback_value);                  /* 偏差 */
#if INCR_LOCT_SELECT
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))   /* E[k]项 */
                        + (PID->Integral * PID->Error)                      /* E[k-1]项 */
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError)); /* E[k-2]项 */
    PID->PrevError = PID->LastError;                                        /* 存储误差，用于下次计算 */
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
    
    PID->ActualValue = (PID->Proportion * PID->Error)                       /* E[k]项 */
                       + (PID->Integral * PID->SumError)                    /* E[k-1]项 */
                       + (PID->Derivative * (PID->Error - PID->LastError)); /* E[k-2]项 */
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
    return ((int32_t)(PID->ActualValue));                                   /* 返回实际控制数值 */

}
