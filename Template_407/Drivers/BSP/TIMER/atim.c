/**
 ****************************************************************************************************
 * @file        atim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       �߼���ʱ�� ��������
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

#include <stdio.h>
#include "atim.h"
#include "led.h"
#include "stepper_tim.h"
#include "stepper_motor.h"
#include "main.h"
#include "delay.h"

#ifndef TIM_MODE_OC_TOGGLE
/******************************************************************************************/
/* ��ʱ�����þ�� ���� */

/* �߼���ʱ��PWM */
TIM_HandleTypeDef g_atimx_handle;           /* ��ʱ��x��� */
TIM_OC_InitTypeDef g_atimx_oc_chy_handle;   /* ��ʱ�������� */
/******************************************************************************************/

/**
 * @brief       �߼���ʱ��TIMX ͨ��Y ���ָ������PWM ��ʼ������
 * @note
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 168Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 168Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void atim_timx_oc_chy_init(uint16_t arr, uint16_t psc)
{
    ATIM_TIMX_PWM_CHY_CLK_ENABLE();        /* TIMX ʱ��ʹ�� */


    g_atimx_handle.Instance = ATIM_TIMX_PWM;                    /* ��ʱ��x */
    g_atimx_handle.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    g_atimx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ���ϼ���ģʽ */
    g_atimx_handle.Init.Period = arr;                           /* �Զ���װ��ֵ */
    g_atimx_handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* ��Ƶ���� */
    g_atimx_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /*ʹ��TIMx_ARR���л���*/
    g_atimx_handle.Init.RepetitionCounter = 0;                  /* ��ʼʱ������*/
    HAL_TIM_PWM_Init(&g_atimx_handle);                          /* ��ʼ��PWM */

    g_atimx_oc_chy_handle.OCMode = TIM_OCMODE_PWM1;             /* ģʽѡ��PWM1 */
    g_atimx_oc_chy_handle.Pulse = arr/4;
    g_atimx_oc_chy_handle.OCPolarity = TIM_OCPOLARITY_HIGH;     /* ����Ƚϼ���Ϊ�� */
    g_atimx_oc_chy_handle.OCNPolarity = TIM_OCPOLARITY_HIGH;
    g_atimx_oc_chy_handle.OCFastMode = TIM_OCFAST_DISABLE;
    g_atimx_oc_chy_handle.OCIdleState = TIM_OCIDLESTATE_RESET;
    g_atimx_oc_chy_handle.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH1); /* ����TIMxͨ��y */

    g_atimx_oc_chy_handle.Pulse = arr/8;
    g_atimx_oc_chy_handle.OCPolarity = TIM_OCPOLARITY_HIGH;
    g_atimx_oc_chy_handle.OCNPolarity = TIM_OCPOLARITY_LOW;

    HAL_TIM_PWM_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_ConfigChannel(&g_atimx_handle, &g_atimx_oc_chy_handle, ATIM_TIMX_PWM_CH4); /* ����TIMxͨ��y */

}


/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ATIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        ATIM_TIMX_PWM_CHY_CLK_ENABLE(); /* ����ͨ��y��CPIOʱ�� */
        ATIM_TIMX_PWM_CH1_GPIO_CLK_ENABLE();   /* IOʱ��ʹ�� */
        ATIM_TIMX_PWM_CH2_GPIO_CLK_ENABLE();   /* IOʱ��ʹ�� */
        ATIM_TIMX_PWM_CH3_GPIO_CLK_ENABLE();   /* IOʱ��ʹ�� */
        ATIM_TIMX_PWM_CH4_GPIO_CLK_ENABLE();   /* IOʱ��ʹ�� */

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH1_GPIO_PIN;         /* ͨ��y��CPIO�� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                    /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLUP;                        /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
        gpio_init_struct.Alternate = ATIM_TIMX_PWM_CHY_GPIO_AF;    /* �˿ڸ��� */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH2_GPIO_PIN;         /* ͨ��y��CPIO�� */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH2_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH3_GPIO_PIN;         /* ͨ��y��CPIO�� */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH3_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_PWM_CH4_GPIO_PIN;         /* ͨ��y��CPIO�� */
        HAL_GPIO_Init(ATIM_TIMX_PWM_CH4_GPIO_PORT, &gpio_init_struct);

#ifdef ENCODER_SIMULATOR_MODE
        HAL_NVIC_SetPriority(ATIM_TIMX_INT_IRQn, 12, 0);
        HAL_NVIC_EnableIRQ(ATIM_TIMX_INT_IRQn);
#endif
        printf("Initialize TIM_PWM finished\r\n");
    }
}


void ATIM_TIMX_INT_IRQHandler(void)
{
	//stepper_stop(STEPPER_MOTOR_1);
	//stepper_stop(STEPPER_MOTOR_2);
	static int cnt = 0;
	__IO uint32_t tim_count = 0;
	cnt++;
	if(cnt == 10)
	{
		tim_count = __HAL_TIM_GET_COUNTER(&g_atimx_handle);
		cnt = 0;
		printf("interrupt reached 10 times \r\n");
		printf("current counter value is %ud \r\n", tim_count);
	}

//	stepper_star(STEPPER_MOTOR_1);
//
//	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_RESET);
//	delay_ms(8);
//	stepper_star(STEPPER_MOTOR_2);

	HAL_TIM_IRQHandler(&g_atimx_handle); /* ��ʱ�����ô����� */
}


#endif

