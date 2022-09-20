/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211015
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "sys.h"

/******************************************************************************************/
//编码器相关的结构体
typedef struct Encoder{
	int 	encode_old;			//前一次编码器计数值
	int 	encode_now;			//当前编码器计数值
	int 	g_speed;			//速度，位置环暂不使用
	int32_t motor_pwm;			//PID期望值，编码器值
	__IO int timx_encode_count;	//编码器定时器溢出次数
	uint8_t g_run_flag;			//电机运行指示标志位

}Encoder_t;



/* 通用定时器 定义 */
/* TIMX 中断定义
 * 默认是针对TIM2~TIM5.
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM8任意一个定时器.
 */

#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

/* TIMX ENCODER输出定义
 * 默认是针对TIM2~TIM5
 * 注意: 通过修改这几个宏定义,可以支持TIM1~TIM8任意一个定时器,任意一个IO口输出PWM
 */
#define GTIM_TIMX_ENCODER_CH1_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH1_GPIO_PIN          GPIO_PIN_6
#define GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)  /* PA口时钟使能 */
#define GTIM_TIMX_ENCODERCH1_GPIO_AF            GPIO_AF2_TIM3                               /* 端口复用到TIM2 */

#define GTIM_TIMX_ENCODER_CH2_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH2_GPIO_PIN          GPIO_PIN_7
#define GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)  /* PB口时钟使能 */
#define GTIM_TIMX_ENCODERCH2_GPIO_AF            GPIO_AF2_TIM3                               /* 端口复用到TIM2 */

/* TIMX REMAP设置
 * 必须通过开启TIM4的部分重映射功能
 * 因此, 必须实现GTIM_TIMX_PWM_CHY_GPIO_REMAP
 * 对那些使用默认设置的定时器PWM输出脚, 不用设置重映射, 是不需要该函数的!
 */

#define GTIM_TIMX_ENCODER                       TIM3                                        /* TIM2 */
#define GTIM_TIMX_ENCODER_CH1                   TIM_CHANNEL_1                                /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH1_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM2 时钟使能 */

#define GTIM_TIMX_ENCODER_CH2                   TIM_CHANNEL_2                                /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH2_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM2 时钟使能 */

/******************************************************************************************/



void gtim_timx_encoder_chy_init(uint16_t arr, uint16_t psc);        /* 通用定时器 定时中断初始化函数 */
int gtim_get_encode(void);

extern Encoder_t g_encoder_t;

#endif

















