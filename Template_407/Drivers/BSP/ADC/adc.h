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

#define ADC_ADCX_CHY_GPIO_PORT              GPIOA
#define ADC_ADCX_CHY_GPIO_PIN               GPIO_PIN_3
#define ADC_ADCX_CHY_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */

#define ADC_ADCX                            ADC1 
#define ADC_ADCX_CHY                        ADC_CHANNEL_3
#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)              /* ADC1 ʱ��ʹ�� */

/* ADC��ͨ��/��ͨ�� DMA�ɼ� DMA��ͨ�� ���� */
#define ADC_ADCX_DMASx                      DMA2_Stream4
#define ADC_ADCX_DMASx_Chanel               DMA_CHANNEL_0
#define ADC_ADCX_DMASx_IRQn                 DMA2_Stream4_IRQn
#define ADC_ADCX_DMASx_IRQHandler           DMA2_Stream4_IRQHandler

#define ADC_ADCX_DMASx_IS_TC()              ( __HAL_DMA_GET_FLAG(&g_dma_adc_handle, DMA_FLAG_TCIF0_4) )  /* �ж�DMA2 Stream4������ɱ�־, ����һ���ٺ�����ʽ,
                                                                                                          * ���ܵ�����ʹ��, ֻ������if���������
                                                                                                          */
#define ADC_ADCX_DMASx_CLR_TC()             do{ __HAL_DMA_CLEAR_FLAG(&g_dma_adc_handle, DMA_FLAG_TCIF0_4); }while(0)   /* ���DMA2 Stream4������ɱ�־ */

/******************************************************************************************/

void adc_dma_init(uint32_t mar);                                                                    /* ADC DMA�ɼ���ʼ�� */
void adc_dma_enable( uint16_t cndtr);                                                               /* ʹ��һ��ADC DMA�ɼ����� */

#endif 















