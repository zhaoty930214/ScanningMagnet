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


/* ADC��ͨ������GPIO���ű�ţ��Լ���������������֮���ӳ���ϵ
 * ADC1_CHANNEL0 ---> PA0 ---> X+
 * ADC1_CHANNEL3 ---> PA3 ---> X-
 * ADC1_CHANNEL6 ---> PA6 ---> Temperature
 * ADC1_CHANNEL8 ---> PB0 ---> Z+
 * ADC3_CHANNEL5 ---> PF7 ---> Z-
 * ADC3_CHANNEL6 ---> PA8 ---> Y+
 * ADC3_CHANNEL7 ---> PA9 ---> Y-
 * */
/* ADC7��ͨ����GPIO���ű��֮���ӳ���ϵ*/
#define ADC_ADC1                            ADC1
#define ADC_ADC1_CH1                        ADC_CHANNEL_0
#define ADC_ADC1_CH2                        ADC_CHANNEL_3
#define ADC_ADC1_CH3                        ADC_CHANNEL_6
#define ADC_ADC1_CH4						ADC_CHANNEL_8

#define ADC_ADC3							ADC3
#define ADC_ADC3_CH5						ADC_CHANNEL_5
#define ADC_ADC3_CH6						ADC_CHANNEL_6
#define ADC_ADC3_CH7						ADC_CHANNEL_7


#define ADC_ADC3_CH5_GPIO_PORT				GPIOF
#define ADC_ADC3_CH5_GPIO_PIN				GPIO_PIN_7
#define ADC_ADC3_CH5_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

#define ADC_ADC3_CH6_GPIO_PORT				GPIOF
#define ADC_ADC3_CH6_GPIO_PIN				GPIO_PIN_8
#define ADC_ADC3_CH6_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

#define ADC_ADC3_CH7_GPIO_PORT				GPIOF
#define ADC_ADC3_CH7_GPIO_PIN				GPIO_PIN_9
#define ADC_ADC3_CH7_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)



#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)              /* ADC1 ʱ��ʹ�� */
#define ADC3_CHY_CLK_ENABLE()				do{ __HAL_RCC_ADC3_CLK_ENABLE(); }while(0)


 /* ADC��ͨ��/��ͨ�� DMA�ɼ� DMA��ͨ�� ���� */
#define ADC_COLL                            1000                                                /* ���ɼ����� */
#define ADC_SUM                             ADC_CH_NUM * ADC_COLL                               /* �ܲɼ����� */
#define ADC_DMA_BUF_SIZE        			100         /* ADC DMA�ɼ� BUF��С */
#define ADC3_DMA_BUF_SIZE       			75          /* ADC DMA�ɼ� BUF��С */

#define ADC3_CH_NUM                         3
#define ADC3_DMASx							DMA2_Stream0
#define ADC_ADC3_DMASx_Chanel             	DMA_CHANNEL_2
#define ADC_ADC3_DMASx_IRQn                 DMA2_Stream0_IRQn
#define ADC_ADC3_DMASx_IRQHandler           DMA2_Stream0_IRQHandler
#define ADC_ADC3_DMASx_IS_TC()              ( __HAL_DMA_GET_FLAG(&g_dma_adc3_handle, DMA_FLAG_TCIF0_4) )  /* �ж�DMA2 Stream4������ɱ�־, ����һ���ٺ�����ʽ*/
#define ADC_ADC3_DMASx_CLR_TC()             do{ __HAL_DMA_CLEAR_FLAG(&g_dma_adc3_handle, DMA_FLAG_TCIF0_4); }while(0)   /* ���DMA2 Stream0������ɱ�־ */

#define ADC1_CH_NUM                         4 		//1
#define ADC1_DMASx                      	DMA2_Stream4
#define ADC_ADC1_DMASx_Chanel             	DMA_CHANNEL_0
#define ADC_ADC1_DMASx_IRQn                 DMA2_Stream4_IRQn
#define ADC_ADC1_DMASx_IRQHandler           DMA2_Stream4_IRQHandler

#define ADC_ADCX_DMASx_IS_TC()              ( __HAL_DMA_GET_FLAG(&g_dma_adc_handle, DMA_FLAG_TCIF0_4) )  /* �ж�DMA2 Stream4������ɱ�־, ����һ���ٺ�����ʽ,*/
#define ADC_ADCX_DMASx_CLR_TC()             do{ __HAL_DMA_CLEAR_FLAG(&g_dma_adc_handle, DMA_FLAG_TCIF0_4); }while(0)   /* ���DMA2 Stream4������ɱ�־ */

/******************************************************************************************/

/* DMA����״̬��־, 0,δ���; 1, ����� */
extern uint8_t g_adc_dma_sta;
extern uint8_t g_adc3_dma_sta;

void adc_instance1_dma_init(uint32_t mar);
void adc_instance2_dma_init(uint32_t mar);
void adc_nch_dma_init(void);           /* ADC DMA�ɼ���ʼ�� */
void adc_dma_enable( uint16_t cndtr);  /* ʹ��һ��ADC DMA�ɼ����� */
void adc3_dma_enable( uint16_t cndtr);  /* ʹ��һ��ADC DMA�ɼ����� */
//

#endif 















