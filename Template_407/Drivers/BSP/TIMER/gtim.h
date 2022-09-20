/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       ͨ�ö�ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211015
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "sys.h"

/******************************************************************************************/
//��������صĽṹ��
typedef struct Encoder{
	int 	encode_old;			//ǰһ�α���������ֵ
	int 	encode_now;			//��ǰ����������ֵ
	int 	g_speed;			//�ٶȣ�λ�û��ݲ�ʹ��
	int32_t motor_pwm;			//PID����ֵ��������ֵ
	__IO int timx_encode_count;	//��������ʱ���������
	uint8_t g_run_flag;			//�������ָʾ��־λ

}Encoder_t;



/* ͨ�ö�ʱ�� ���� */
/* TIMX �ж϶���
 * Ĭ�������TIM2~TIM5.
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM8����һ����ʱ��.
 */

#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 ʱ��ʹ�� */

/* TIMX ENCODER�������
 * Ĭ�������TIM2~TIM5
 * ע��: ͨ���޸��⼸���궨��,����֧��TIM1~TIM8����һ����ʱ��,����һ��IO�����PWM
 */
#define GTIM_TIMX_ENCODER_CH1_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH1_GPIO_PIN          GPIO_PIN_6
#define GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)  /* PA��ʱ��ʹ�� */
#define GTIM_TIMX_ENCODERCH1_GPIO_AF            GPIO_AF2_TIM3                               /* �˿ڸ��õ�TIM2 */

#define GTIM_TIMX_ENCODER_CH2_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH2_GPIO_PIN          GPIO_PIN_7
#define GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)  /* PB��ʱ��ʹ�� */
#define GTIM_TIMX_ENCODERCH2_GPIO_AF            GPIO_AF2_TIM3                               /* �˿ڸ��õ�TIM2 */

/* TIMX REMAP����
 * ����ͨ������TIM4�Ĳ�����ӳ�书��
 * ���, ����ʵ��GTIM_TIMX_PWM_CHY_GPIO_REMAP
 * ����Щʹ��Ĭ�����õĶ�ʱ��PWM�����, ����������ӳ��, �ǲ���Ҫ�ú�����!
 */

#define GTIM_TIMX_ENCODER                       TIM3                                        /* TIM2 */
#define GTIM_TIMX_ENCODER_CH1                   TIM_CHANNEL_1                                /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH1_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM2 ʱ��ʹ�� */

#define GTIM_TIMX_ENCODER_CH2                   TIM_CHANNEL_2                                /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH2_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM2 ʱ��ʹ�� */

/******************************************************************************************/



void gtim_timx_encoder_chy_init(uint16_t arr, uint16_t psc);        /* ͨ�ö�ʱ�� ��ʱ�жϳ�ʼ������ */
int gtim_get_encode(void);

extern Encoder_t g_encoder_t;

#endif

















