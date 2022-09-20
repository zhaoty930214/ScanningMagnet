/**
 ****************************************************************************************************
 * @file        stepper_tim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       定时器 驱动代码
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
 * V1.0 20211019
 * 第一次发布
 *
 ****************************************************************************************************
 */


#include "stepper_tim.h"
#include "led.h"
#include "main.h"
#include <stdio.h>

#ifdef TIM_MODE_OC_TOGGLE
/******************************************************************************************/
/* 定时器配置句柄 定义 */

/* 高级定时器PWM */
TIM_HandleTypeDef g_atimx_handle;           /* 定时器x句柄 */
TIM_OC_InitTypeDef g_atimx_oc_chy_handle;   /* 定时器输出句柄 */
uint32_t g_count_val = 0;                   /* 计数值 */
uint32_t g_ccr_val = 500;                   /* 比较值变值 */
extern volatile uint16_t g_step_angle;

/******************************************************************************************/

/**
 * @brief       高级定时器TIMX PWM 初始化函数
 * @note
 *              高级定时器的时钟来自APB2, 而PCLK2 = 168Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 168Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void atim_timx_oc_chy_init(uint16_t arr, uint16_t psc)
{
    ATIM_TIMX_PWM_CHY_CLK_ENABLE();                             /* TIMX 时钟使能 */


    g_atimx_handle.Instance = ATIM_TIMX_PWM;                    /* 定时器x */
    g_atimx_handle.Init.Prescaler = psc;                        /* 定时器分频 */
    g_atimx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 向上计数模式 */
    g_atimx_handle.Init.Period = arr;                           /* 自动重装载值 */
    g_atimx_handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* 分频因子 */
    g_atimx_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /*使能TIMx_ARR进行缓冲*/
    g_atimx_handle.Init.RepetitionCounter = 0;                  /* 开始时不计数*/
    HAL_TIM_OC_Init(&g_atimx_handle);                           /* 初始化PWM */

    g_atimx_oc_chy_handle.OCMode = TIM_OCMODE_TOGGLE;           /* 翻转模式 */
    g_atimx_oc_chy_handle.Pulse = 0;
    g_atimx_oc_chy_handle.OCPolarity = TIM_OCPOLARITY_HIGH;     /* 输出比较极性为高 */
    g_atimx_oc_chy_handle.OCNPolarity = TIM_OCNPOLARITY_LOW;
    g_atimx_oc_chy_handle.OCFastMode = TIM_OCFAST_DISABLE;
    g_atimx_oc_chy_handle.OCIdleState = TIM_OCIDLESTATE_RESET;
    g_atimx_oc_chy_handle.OCNIdleState = TIM_OCNIDLESTATE_RESET;

#ifdef ENCODER_SIMULATOR_MODE
    g_atimx_oc_chy_handle.Pulse = 0x6000;
#endif
    HAL_TIM_OC_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_OC_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_OC_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_OC_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH4); /* 配置TIMx通道y */

    HAL_TIM_Base_Start(&g_atimx_handle);
}


/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_OC_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ATIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        ATIM_TIMX_PWM_CHY_CLK_ENABLE();                             /* 开启通道y的GPIO时钟 */
        ATIM_TIMX_PWM_CH1_GPIO_CLK_ENABLE();                        /* IO时钟使能 */
        ATIM_TIMX_PWM_CH2_GPIO_CLK_ENABLE();                        /* IO时钟使能 */
        ATIM_TIMX_PWM_CH3_GPIO_CLK_ENABLE();                        /* IO时钟使能 */
        ATIM_TIMX_PWM_CH4_GPIO_CLK_ENABLE();                        /* IO时钟使能 */

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH1_GPIO_PIN;          /* 通道y的GPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                    /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                        /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
        gpio_init_struct.Alternate = ATIM_TIMX_PWM_CHY_GPIO_AF;     /* 端口复用 */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH2_GPIO_PIN;          /* 通道y的GPIO口 */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH2_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH3_GPIO_PIN;          /* 通道y的GPIO口 */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH3_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH4_GPIO_PIN;          /* 通道y的GPIO口 */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH4_GPIO_PORT, &gpio_init_struct);


        /*设置TIM8比较捕获中断优先级为7，此中断用于步进电机的PUL信号，其优先级应该低于PID中断检测的优先级*/
        HAL_NVIC_SetPriority(ATIM_TIMX_INT_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(ATIM_TIMX_INT_IRQn);
    }
}

/**
 * @brief       高级定时器TIMX NPWM中断服务函数
 * @param       无
 * @retval      无
 */
void ATIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_atimx_handle); /* 定时器共用处理函数 */
}


extern int g_pulse_count;
/**
  * @brief  定时器比较中断
  * @param  htim：定时器句柄指针
  * @note   无
  * @retval 无
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    __IO uint32_t tim_count = 0;
    __IO uint32_t tmp = 0;
    if(htim->Instance == ATIM_TIMX_PWM)
    {
    	//g_pulse_count--;
        tim_count = __HAL_TIM_GET_COUNTER(&g_atimx_handle);
        tmp = (tim_count + g_step_angle) & 0xFFFF;
        __HAL_TIM_SetCompare(&g_atimx_handle,ATIM_TIMX_PWM_CH1, tmp);

//        if(g_pulse_count < 0)
//        {
//        	HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);
//        }
    }
}

#endif
