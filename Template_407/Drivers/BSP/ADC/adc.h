/**
 ****************************************************************************************************
 * @file        adc.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-18
 * @brief       ADC ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com/forum.php
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211018
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __ADC_H
#define __ADC_H

#include "sys.h"


/******************************************************************************************/
/* ADC������ ���� */

#define ADC_ADCX_CH1_GPIO_PORT              GPIOA
#define ADC_ADCX_CH1_GPIO_PIN               GPIO_PIN_0
#define ADC_ADCX_CH1_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */

#define ADC_ADCX_CH2_GPIO_PORT				GPIOA
#define ADC_ADCX_CH2_GPIO_PIN				GPIO_PIN_3
#define ADC_ADCX_CH2_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define ADC_ADCX_CH3_GPIO_PORT				GPIOA
#define ADC_ADCX_CH3_GPIO_PIN				GPIO_PIN_6
#define ADC_ADCX_CH3_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define ADC_ADCX_CH4_GPIO_PORT				GPIOB
#define ADC_ADCX_CH4_GPIO_PIN				GPIO_PIN_0
#define ADC_ADCX_CH4_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)



#define ADC_ADC1                            ADC1
#define ADC_ADCX_CH1                        ADC_CHANNEL_0
#define ADC_ADCX_CH2                        ADC_CHANNEL_3
#define ADC_ADCX_CH3                        ADC_CHANNEL_6
#define ADC_ADCX_CH4						ADC_CHANNEL_8

#define ADC_ADC2							ADC3
#define ADC_ADCX_CH5						ADC_CHANNEL_5
#define ADC_ADCX_CH6						ADC_CHANNEL_6
#define ADC_ADCX_CH7						ADC_CHANNEL_7


#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)              /* ADC1 ʱ��ʹ�� */

//#define ADC_NUM		3
#define ADC1_CH_NUM                         4 		//1                                                   /* ��Ҫת����ͨ����Ŀ */
#define ADC_COLL                            1000                                                /* ���ɼ����� */
#define ADC_SUM                             ADC_CH_NUM * ADC_COLL                               /* �ܲɼ����� */
#define ADC_DMA_BUF_SIZE        100         /* ADC DMA�ɼ� BUF��С */

/* ADC��ͨ��/��ͨ�� DMA�ɼ� DMA��ͨ�� ���� */
#define ADC_ADCX_DMASx                      DMA2_Stream4
#define ADC_ADCX_DMASx_Chanel               DMA_CHANNEL_0
#define ADC_ADCX_DMASx_IRQn                 DMA2_Stream4_IRQn
#define ADC_ADCX_DMASx_IRQHandler           DMA2_Stream4_IRQHandler

#define ADC_ADCX_DMASx_IS_TC()              ( __HAL_DMA_GET_FLAG(&g_dma_adc_handle, DMA_FLAG_TCIF0_4) )  /* �ж�DMA2 Stream4������ɱ�־, ����һ���ٺ�����ʽ,
                                                                                                          * ���ܵ�����ʹ��, ֻ������if���������
                                                                                                          */
#define ADC_ADCX_DMASx_CLR_TC()             do{ __HAL_DMA_CLEAR_FLAG(&g_dma_adc_handle, DMA_FLAG_TCIF0_4); }while(0)   /* ���DMA2 Stream4������ɱ�־ */

#define ADC_BUF_LEN 100
/******************************************************************************************/

extern uint8_t g_adc_dma_sta;

void adc_dma_init(uint32_t mar);
void adc_nch_dma_init(void);                                                                    /* ADC DMA�ɼ���ʼ�� */
void adc_dma_enable( uint16_t cndtr);                                                               /* ʹ��һ��ADC DMA�ɼ����� */
//

#endif 















