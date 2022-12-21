/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211015
 * 第一次发布
 * V1.1 20211015
 * 新增gtim_timx_pwm_chy_init函数
 ****************************************************************************************************
 */

#include "gtim.h"
#include "led.h"
//#include "pid.h"
#include "stdlib.h"
#include "stepper_motor.h"
#include "btim.h"
#include "pid.h"
#include <stdio.h>
#include <stdbool.h>
/******************************************************************************************/

/* 定时器配置句柄 定义 */
TIM_HandleTypeDef g_timx_handle;             /* 定时器x句柄 */

/*定时器PWM*/
static TIM_HandleTypeDef g_timx_encode_chy_handle;     /* 定时器x句柄 */
//TIM_OC_InitTypeDef g_timx_oc_pwm_chy_handle; /* 定时器输出句柄 */
static TIM_Encoder_InitTypeDef g_timx_encoder_chy_handle; /* 定时器编码器句柄 */
//uint8_t g_timxchy_cap_sta = 0;               /* 输入捕获状态 */
//uint16_t g_timxchy_cap_val = 0;              /* 输入捕获值 */


/*---------------------------编码器相关参数，全局变量---------------------------*/
Encoder_t g_encoder_t;

/******************************************************************************************/


//volatile int g_timx_encode_count = 0;

/**
 * @brief       通用定时器TIMX定时中断初始化函数
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE(); /* 使能TIMx时钟 */

    g_timx_handle.Instance = GTIM_TIMX_INT;                     /* 通用定时器x */
    g_timx_handle.Init.Prescaler = psc;                         /* 分频 */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 向上计数器 */
    g_timx_handle.Init.Period = arr;                            /* 自动装载值 */
    g_timx_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  /* 时钟分频因子 */
    HAL_TIM_Base_Init(&g_timx_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 3);             /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                     /* 开启ITMx中断 */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                      /* 使能定时器x和定时器x更新中断 */
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_encode_chy_handle);
}

/*********************************通用定时器encoder程序*************************************/
/**
 * @brief       通用定时器TIMX 通道Y 编码器接口模式 初始化函数
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void gtim_timx_encoder_chy_init(uint16_t arr, uint16_t psc)
{
    g_timx_encode_chy_handle.Instance = GTIM_TIMX_ENCODER;                 /* 定时器x */
    g_timx_encode_chy_handle.Init.Prescaler = psc;                         /* 定时器分频 */
    g_timx_encode_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 向上计数模式 */
    g_timx_encode_chy_handle.Init.Period = arr;                            /* 自动重装载值 */
    g_timx_encode_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  /* 时钟分频因子 */
    g_timx_encoder_chy_handle.EncoderMode = TIM_ENCODERMODE_TI12;          /* TI1,TI2都检测 */
    g_timx_encoder_chy_handle.IC1Polarity = TIM_ICPOLARITY_RISING;         /* 输入极性，非反向 */
    g_timx_encoder_chy_handle.IC1Selection = TIM_ICSELECTION_DIRECTTI;     /* 输入通道选择 */
    g_timx_encoder_chy_handle.IC1Prescaler = TIM_ICPSC_DIV1;               /* 一分频 */
    g_timx_encoder_chy_handle.IC1Filter = 10;                              /* 滤波器设置 */
    g_timx_encoder_chy_handle.IC2Polarity = TIM_ICPOLARITY_RISING;         /* 输入极性，非反向 */
    g_timx_encoder_chy_handle.IC2Selection = TIM_ICSELECTION_DIRECTTI;     /* 输入通道选择 */
    g_timx_encoder_chy_handle.IC2Prescaler = TIM_ICPSC_DIV1;               /* 一分频 */
    g_timx_encoder_chy_handle.IC2Filter = 10;                              /* 滤波器设置 */
    HAL_TIM_Encoder_Init(&g_timx_encode_chy_handle, &g_timx_encoder_chy_handle);

    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle,GTIM_TIMX_ENCODER_CH1);
    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle,GTIM_TIMX_ENCODER_CH2);
    __HAL_TIM_CLEAR_FLAG(&g_timx_encode_chy_handle,TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&g_timx_encode_chy_handle,TIM_IT_UPDATE);

}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_Encoder_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *tim_encoderHandle)
{
    if (tim_encoderHandle->Instance == GTIM_TIMX_ENCODER)
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE();                            /* 开启通道y的CPIO时钟 */
        GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE();
        GTIM_TIMX_ENCODER_CH1_CLK_ENABLE();                                 /* 开启定时器时钟 */
        GTIM_TIMX_ENCODER_CH2_CLK_ENABLE();

        gpio_init_struct.Pin = GTIM_TIMX_ENCODER_CH1_GPIO_PIN;              /* 通道y的CPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_struct.Pull = GPIO_NOPULL;                            /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;                  /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_ENCODERCH1_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIMX_ENCODER_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = GTIM_TIMX_ENCODER_CH2_GPIO_PIN;              /* 通道y的CPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_struct.Pull = GPIO_NOPULL;                            /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;                  /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_ENCODERCH2_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIMX_ENCODER_CH2_GPIO_PORT, &gpio_init_struct);

        //定时器编码器模式的中断优先级，应该高于PID检测的优先级，及时更新溢出中断次数
        HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 15, 0);				//NVIC中断分组为Group4，中断优先级设置为5，高于PWM中断，低于PID周期中断
        HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);

    }
}

/**
 * @brief       获取编码器的值
 * @param       无
 * @retval      编码器值
 */
int gtim_get_encode(void)
{
    return ( int32_t )(__HAL_TIM_GET_COUNTER(&g_timx_encode_chy_handle) + g_encoder_t.timx_encode_count * 65536);
}



/**
 * @brief       定时器更新中断回调函数，此中断回调函数，由TIM3和TIM6共用;TIM3记录溢出次数，
 * 				TIM6定时调用PID
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t val=0;
    static bool flag_match_debug =false;
    if(htim->Instance == GTIM_TIMX_ENCODER)
    {
    	g_encoder_t.timx_encode_count += __HAL_TIM_IS_TIM_COUNTING_DOWN(&g_timx_encode_chy_handle)?
    									 -1:1;
//        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&g_timx_encode_chy_handle))
//        {
//        	g_encoder_t.timx_encode_count--;
//        }
//        else
//        {
//        	g_encoder_t.timx_encode_count++;
//        }
    }
    else if(htim->Instance == BTIM_TIMX_INT)
    {
    	g_encoder_t.encode_now = gtim_get_encode();        /* 获取编码器值，用于计算速度 */
        if(val == SMAPLSE_PID_PERIOD)
        {
        	g_encoder_t.g_speed = g_encoder_t.encode_now-g_encoder_t.encode_old;	//计算编码值变化量

        	g_encoder_t.encode_old=g_encoder_t.encode_now;				//刷新编码值记录

            if(g_encoder_t.g_run_flag)/* 电机启动 */
            {
                g_step_motor.location=gtim_get_encode();
                //increment_pid_ctrl的返回值是相较于前一次输出目标值的变化量，大于0表示需要增加，小于0表示需要较少
                g_encoder_t.motor_pwm = increment_pid_ctrl(&g_location_pid, g_step_motor.location);


//            	printf("increment_pid_ctrl return value %d @setpoint value %d\r\n",
//            			g_encoder_t.motor_pwm,
//						g_location_pid.SetPoint);


                if( 0!= g_encoder_t.motor_pwm)
                {
                	flag_match_debug = false;
                	//printf("increment_pid_ctrl returned value %ld\r\n", g_encoder_t.motor_pwm);
                }
                else
                {
                	//编码器值匹配成功，可以关闭定时器了
                	if(flag_match_debug == false)
                	{
                		flag_match_debug = true;
                		//printf("increment_pid_ctrl return 0!\r\n");
                		stepper_stop(STEPPER_MOTOR_1);
                	}
                }
                (g_encoder_t.motor_pwm > 0) ? (g_step_motor.dir=CW) : (g_step_motor.dir=CCW);
                g_encoder_t.motor_pwm = abs(g_encoder_t.motor_pwm);
                 /*
                    限制最大20ms的时间内步进电机只能接收150*1600/2400=100个脉冲  限制速度
                    20ms最多输出100个脉冲，实际就是1ms输出5个脉冲也就是1s输出5000个脉冲
                    电机旋转一圈需要1600个脉冲也就是 5000/1600 = 3.125转每秒
                    换算成rpm的话：3.125*60 = 187.5rpm（转每分钟）
                */
                stepper_motion_ctrl(g_step_motor.dir, g_encoder_t.motor_pwm);
                //g_send_flag=1;
            }
            val = 0;
        }
        val ++;
    }
}



