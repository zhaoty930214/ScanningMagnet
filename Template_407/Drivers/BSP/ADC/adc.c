/**
 ****************************************************************************************************
 * @file        adc.c
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

#include "adc.h"
#include "delay.h"


DMA_HandleTypeDef g_dma_adc_handle = {0};                                   /* ����Ҫ����ADC���ݵ�DMA��� */
ADC_HandleTypeDef g_adc_dma_handle = {0};                                   /* ����ADC��DMA��ȡ����� */
uint8_t g_adc_dma_sta = 0;                                                  /* DMA����״̬��־, 0,δ���; 1, ����� */

/**
 * @brief       ADC DMA��ȡ ��ʼ������
 * @param       mar         : �洢����ַ
 * @retval      ��
 */
void adc_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    ADC_ADCX_CHY_CLK_ENABLE();                                              /* ʹ��ADCxʱ�� */
    ADC_ADCX_CH1_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */
    ADC_ADCX_CH2_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */
    ADC_ADCX_CH3_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */
    ADC_ADCX_CH4_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */

    if ((uint32_t)ADC_ADCX_DMASx > (uint32_t)DMA2)                          /* ����DMA2�Ļ���ַ, ��ΪDMA2��������ͨ���� */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                                        /* DMA2ʱ��ʹ�� */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                                        /* DMA1ʱ��ʹ�� */
    }

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = ADC_ADCX_CH1_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ADC_ADCX_CH1_GPIO_PORT, &gpio_init_struct);

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = ADC_ADCX_CH2_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ADC_ADCX_CH2_GPIO_PORT, &gpio_init_struct);

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = ADC_ADCX_CH3_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ADC_ADCX_CH3_GPIO_PORT, &gpio_init_struct);

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = ADC_ADCX_CH4_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ADC_ADCX_CH4_GPIO_PORT, &gpio_init_struct);


    /* ��ʼ��DMA */
    g_dma_adc_handle.Instance = ADC_ADCX_DMASx;                             /* ����DMA������ */
    g_dma_adc_handle.Init.Channel = DMA_CHANNEL_0;                          /* ����DMAͨ�� */
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;                 /* �����赽�洢��ģʽ */
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                     /* ���������ģʽ */
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                         /* �洢������ģʽ */
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* �������ݳ���:16λ */
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* �洢�����ݳ���:16λ */
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                                /* ��������ģʽ */
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_adc_handle);

    g_adc_dma_handle.Instance = ADC_ADC1;
    g_adc_dma_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;    /* 4��Ƶ��21Mhz */
    g_adc_dma_handle.Init.Resolution = ADC_RESOLUTION_12B;                  /* 12λģʽ */
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                  /* �Ҷ��� */
    g_adc_dma_handle.Init.ScanConvMode = ENABLE;                           /* ��ɨ��ģʽ */
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;                      /* ��������ת�� */
    g_adc_dma_handle.Init.NbrOfConversion = ADC1_CH_NUM;                              /* ��ʵ���õ�1������ͨ������ */
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;                  /* ��ֹ����������ģʽ */
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;                          /* ����������ͨ����Ϊ0 */
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;            /* ������� */
    g_adc_dma_handle.Init.DMAContinuousRequests = ENABLE;                   /* ����DMA���� */
    HAL_ADC_Init(&g_adc_dma_handle);                                        /* ��ʼ��ADC */

    __HAL_LINKDMA(&g_adc_dma_handle, DMA_Handle, g_dma_adc_handle);         /* ��ADC��DMA�������� */

    /* *************����ADCͨ��, ǰ�ĸ�ͨ��������ADC1��ͨ����������ͨ��������ADC3��ͨ��**************** */
    adc_ch_conf.Channel = ADC_ADCX_CH1;                                     /* ͨ�� */
    adc_ch_conf.Rank = 1;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */

    adc_ch_conf.Channel = ADC_ADCX_CH2;                                     /* ͨ�� */
    adc_ch_conf.Rank = 2;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */

    adc_ch_conf.Channel = ADC_ADCX_CH3;                                     /* ͨ�� */
    adc_ch_conf.Rank = 3;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */


    adc_ch_conf.Channel = ADC_ADCX_CH4;                                     /* ͨ�� */
    adc_ch_conf.Rank = 4;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */
    /* *************����ADCͨ��, ǰ�ĸ�ͨ��������ADC1��ͨ����������ͨ��������ADC3��ͨ��**************** */


//    adc_ch_conf.Channel = ADC_ADCX_CH3;                                     /* ͨ�� */
//    adc_ch_conf.Rank = 2;                                                   /* ���� */
//    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
//    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */
//
//    adc_ch_conf.Channel = ADC_ADCX_CH6;                                     /* ͨ�� */
//    adc_ch_conf.Rank = 3;                                                   /* ���� */
//    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
//    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */
//
//    adc_ch_conf.Channel = ADC_ADCX_CH6;                                     /* ͨ�� */
//    adc_ch_conf.Rank = 3;                                                   /* ���� */
//    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
//    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* ͨ������ */


    /* ����DMA�����������ж����ȼ� */
    HAL_NVIC_SetPriority(ADC_ADCX_DMASx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMASx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0);       /* ����DMA���������ж� */
    HAL_ADC_Start_DMA(&g_adc_dma_handle,&mar,0);                            /* ����ADC��ͨ��DMA������ */
}

/**
 * @brief       ʹ��һ��ADC DMA����
 * @param       cndtr: DMA����Ĵ���
 * @retval      ��
 */
void adc_dma_enable(uint16_t cndtr)
{
    __HAL_ADC_DISABLE(&g_adc_dma_handle);           /* �ȹر�ADC */

    __HAL_DMA_DISABLE(&g_dma_adc_handle);           /* �ر�DMA���� */
    g_dma_adc_handle.Instance->NDTR = cndtr;        /* ����DMA���������� */
    __HAL_DMA_ENABLE(&g_dma_adc_handle);            /* ����DMA���� */

    __HAL_ADC_ENABLE(&g_adc_dma_handle);            /* ��������ADC */
    ADC_ADC1->CR2 |= 1 << 30;                       /* ��������ת��ͨ�� */
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void ADC_ADCX_DMASx_IRQHandler(void)
{
    if (ADC_ADCX_DMASx_IS_TC())
    {
        g_adc_dma_sta = 1;                          /* ���DMA������� */
        ADC_ADCX_DMASx_CLR_TC();                    /* ���DMA2 ������4 ��������ж� */
    }
}







