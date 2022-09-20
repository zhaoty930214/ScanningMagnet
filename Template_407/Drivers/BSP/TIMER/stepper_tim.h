/**
 ****************************************************************************************************
 * @file        stepper_tim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       ��ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211019
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __STEPPER_TIM_H
#define __STEPPER_TIM_H

#include "sys.h"

/******************************************************************************************/
/* �߼���ʱ�� ���� */

/* TIMX PWM ���� */
#define ATIM_TIMX_PWM_CH1_GPIO_PORT            GPIOI
#define ATIM_TIMX_PWM_CH1_GPIO_PIN             GPIO_PIN_5
#define ATIM_TIMX_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)     /* PI��ʱ��ʹ�� */

#define ATIM_TIMX_PWM_CH2_GPIO_PORT            GPIOI
#define ATIM_TIMX_PWM_CH2_GPIO_PIN             GPIO_PIN_6
#define ATIM_TIMX_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)     /* PI��ʱ��ʹ�� */

#define ATIM_TIMX_PWM_CH3_GPIO_PORT            GPIOI
#define ATIM_TIMX_PWM_CH3_GPIO_PIN             GPIO_PIN_7
#define ATIM_TIMX_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)     /* PI��ʱ��ʹ�� */

#define ATIM_TIMX_PWM_CH4_GPIO_PORT            GPIOC
#define ATIM_TIMX_PWM_CH4_GPIO_PIN             GPIO_PIN_9
#define ATIM_TIMX_PWM_CH4_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define ATIM_TIMX_PWM_CHY_GPIO_AF              GPIO_AF3_TIM8

#define ATIM_TIMX_PWM                          TIM8
#define ATIM_TIMX_INT_IRQn                     TIM8_CC_IRQn
#define ATIM_TIMX_INT_IRQHandler               TIM8_CC_IRQHandler
#define ATIM_TIMX_PWM_CH1                      TIM_CHANNEL_1                                    /* ͨ��1 */
#define ATIM_TIMX_PWM_CH2                      TIM_CHANNEL_2                                    /* ͨ��2 */
#define ATIM_TIMX_PWM_CH3                      TIM_CHANNEL_3                                    /* ͨ��3 */
#define ATIM_TIMX_PWM_CH4                      TIM_CHANNEL_4                                    /* ͨ��4 */


#define ATIM_TIMX_PWM_IRQn					   TIM8_UP_TIM13_IRQn
#define ATIM_TIMX_PWM_IRQHandler			   TIM8_UP_TIM13_IRQHandler



#define ATIM_TIMX_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)       /* TIM8 ʱ��ʹ�� */


extern TIM_HandleTypeDef g_atimx_handle;                /* ��ʱ��x��� */
extern TIM_OC_InitTypeDef g_atimx_oc_chy_handle;        /* ��ʱ�������� */
/******************************************************************************************/

void atim_timx_oc_chy_init(uint16_t arr, uint16_t psc); /* �߼���ʱ�� PWM��ʼ������ */

#endif

















