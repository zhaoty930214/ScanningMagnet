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

uint8_t g_adc3_dma_sta = 0;
DMA_HandleTypeDef g_dma_adc3_handle = {0};
ADC_HandleTypeDef g_adc3_dma_handle = {0};                                   /* ����ADC��DMA��ȡ����� */

/**
 * @brief       ADC ʵ������2 DMA��ȡ ��ʼ������.ʹ�ܶ�ͨ����������ģʽ+ɨ��ģʽ+DMA����ģʽ��Ŀ��һ��ʹ��7��ADCͨ��������4������ADC
 * 				ʵ��1(ADC1),3������ADCʵ��2(ADC3)��ADC1ʹ��DMA2_Stream4/ADC3ʹ��DMA2_Stream0������ο�https://mevi
 * 				on.atlassian.net/wiki/spaces/YBL/pages/59506920/ADC1+ADC3+DMA
 * @param       mar: �洢����ַ������ADC������ص�����Ҫ�洢���Ļ������ʼ��ַ
 * @retval      ��
 * @Version		����							����				�޸�Ŀ��
 * 1.0			tianyu.zhao@mevion.com		2022-11-16		����
 * 1.1			tianyu.zhao@mevion.com		2022-11-07		https://mevion.atlassian.net/browse/YBL-948
 */
void adc_instance2_dma_init(uint32_t mar)
{
	GPIO_InitTypeDef gpio_init_struct;
	ADC_ChannelConfTypeDef adc_ch_conf = {0};

	ADC3_CHY_CLK_ENABLE();
	ADC_ADC3_CH5_GPIO_CLK_ENABLE();
	ADC_ADC3_CH6_GPIO_CLK_ENABLE();
	ADC_ADC3_CH7_GPIO_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();                                        /* DMA2ʱ��ʹ�� */

    gpio_init_struct.Pin = ADC_ADC3_CH5_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
	gpio_init_struct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(ADC_ADC3_CH5_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = ADC_ADC3_CH6_GPIO_PIN;
	HAL_GPIO_Init(ADC_ADC3_CH6_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = ADC_ADC3_CH7_GPIO_PIN;
	HAL_GPIO_Init(ADC_ADC3_CH7_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��DMA */
	g_dma_adc3_handle.Instance = 				 	ADC3_DMASx;
	g_dma_adc3_handle.Init.Channel = 				ADC_ADC3_DMASx_Chanel;                /* ����DMAͨ�� */
	g_dma_adc3_handle.Init.Direction = 				DMA_PERIPH_TO_MEMORY;                 /* �����赽�洢��ģʽ */
	g_dma_adc3_handle.Init.PeriphInc = 				DMA_PINC_DISABLE;                     /* ���������ģʽ */
	g_dma_adc3_handle.Init.MemInc = 				DMA_MINC_ENABLE;                      /* �洢������ģʽ */
    g_dma_adc3_handle.Init.PeriphDataAlignment = 	DMA_PDATAALIGN_HALFWORD;    		  /* �������ݳ���:16λ */
    g_dma_adc3_handle.Init.MemDataAlignment = 		DMA_MDATAALIGN_HALFWORD;       		  /* �洢�����ݳ���:16λ */
    g_dma_adc3_handle.Init.Mode = 					DMA_NORMAL;                           /* ��������ģʽ */
    g_dma_adc3_handle.Init.Priority =		 		DMA_PRIORITY_MEDIUM;                  /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_adc3_handle);


    g_adc3_dma_handle.Instance = 					ADC_ADC3;
    g_adc3_dma_handle.Init.ClockPrescaler = 		ADC_CLOCKPRESCALER_PCLK_DIV4;    	  /* 4��Ƶ��21Mhz */
    g_adc3_dma_handle.Init.Resolution = 			ADC_RESOLUTION_12B;                   /* 12λģʽ */
    g_adc3_dma_handle.Init.DataAlign = 				ADC_DATAALIGN_RIGHT;                  /* �Ҷ��� */
    g_adc3_dma_handle.Init.ScanConvMode = 			ENABLE;                            	  /* ��ɨ��ģʽ */
    g_adc3_dma_handle.Init.ContinuousConvMode =	 	ENABLE;                      		  /* ��������ת�� */
    g_adc3_dma_handle.Init.NbrOfConversion = 		3;                    				  /* ��ʵ���õ�3������ͨ������ */
    g_adc3_dma_handle.Init.DiscontinuousConvMode = 	DISABLE;                  			  /* ��ֹ����������ģʽ */
    g_adc3_dma_handle.Init.NbrOfDiscConversion = 	0;                          		  /* ����������ͨ����Ϊ0 */
    g_adc3_dma_handle.Init.ExternalTrigConv = 		ADC_SOFTWARE_START;            		  /* ������� */
    g_adc3_dma_handle.Init.DMAContinuousRequests = 	ENABLE;                   			  /* ����DMA���� */
    HAL_ADC_Init(&g_adc3_dma_handle);                                        			  /* ��ʼ��ADC */

    __HAL_LINKDMA(&g_adc3_dma_handle, DMA_Handle, g_dma_adc3_handle);         			  /* ��ADC��DMA�������� */

    /* *************����ADCͨ��, ǰ�ĸ�ͨ��������ADC1��ͨ����������ͨ��������ADC3��ͨ��**************** */
    adc_ch_conf.Channel = ADC_ADCX_CH5;                                     /* ͨ�� */
    adc_ch_conf.Rank = 1;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc3_dma_handle, &adc_ch_conf);                /* ͨ������ */

    adc_ch_conf.Channel = ADC_ADCX_CH6;                                     /* ͨ�� */
    adc_ch_conf.Rank = 2;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc3_dma_handle, &adc_ch_conf);                /* ͨ������ */

    adc_ch_conf.Channel = ADC_ADCX_CH7;                                     /* ͨ�� */
    adc_ch_conf.Rank = 3;                                                   /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc3_dma_handle, &adc_ch_conf);                /* ͨ������ */

    /* *************����ADCͨ��, ǰ�ĸ�ͨ��������ADC1��ͨ����������ͨ��������ADC3��ͨ��**************** */


    /* ����DMA�����������ж����ȼ� */
    HAL_NVIC_SetPriority(ADC_ADC3_DMASx_IRQn, 3, 4);
    HAL_NVIC_EnableIRQ(ADC_ADC3_DMASx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc3_handle, (uint32_t)&ADC3->DR, mar, 0);       /* ����DMA���������ж� */
    HAL_ADC_Start_DMA(&g_adc3_dma_handle,&mar,0);
}

/**
 * @brief       ADC ʵ������1 DMA��ȡ ��ʼ������.ʹ�ܶ�ͨ����������ģʽ+ɨ��ģʽ+DMA����ģʽ��Ŀ��һ��ʹ��7��ADCͨ��������4������ADC
 * 				ʵ��1(ADC1),3������ADCʵ��2(ADC3)��ADC1ʹ��DMA2_Stream4/ADC3ʹ��DMA2_Stream0������ο�https://mevi
 * 				on.atlassian.net/wiki/spaces/YBL/pages/59506920/ADC1+ADC3+DMA
 * @param       mar: �洢����ַ������ADC������ص�����Ҫ�洢���Ļ������ʼ��ַ
 * @retval      ��
 * @Version		����							����				�޸�Ŀ��
 * 1.0			tianyu.zhao@mevion.com		2022-11-16		����
 * 1.1			tianyu.zhao@mevion.com		2022-11-07		https://mevion.atlassian.net/browse/YBL-948
 */
void adc_instance1_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    ADC_ADCX_CHY_CLK_ENABLE();                                              /* ʹ��ADCxʱ�� */
    ADC_ADCX_CH1_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */
    ADC_ADCX_CH2_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */
    ADC_ADCX_CH3_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */
    ADC_ADCX_CH4_GPIO_CLK_ENABLE();                                         /* ����GPIOʱ�� */

    if ((uint32_t)ADC1_DMASx > (uint32_t)DMA2)                          /* ����DMA2�Ļ���ַ, ��ΪDMA2��������ͨ���� */
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
    HAL_GPIO_Init(ADC_ADCX_CH2_GPIO_PORT, &gpio_init_struct);

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = ADC_ADCX_CH3_GPIO_PIN;
    HAL_GPIO_Init(ADC_ADCX_CH3_GPIO_PORT, &gpio_init_struct);

    /* ����AD�ɼ�ͨ����ӦIO���Ź���ģʽ */
    gpio_init_struct.Pin = ADC_ADCX_CH4_GPIO_PIN;
    HAL_GPIO_Init(ADC_ADCX_CH4_GPIO_PORT, &gpio_init_struct);


    /* ��ʼ��DMA */
    g_dma_adc_handle.Instance = ADC1_DMASx;                             /* ����DMA������ */
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
    g_adc_dma_handle.Init.ScanConvMode = ENABLE;                            /* ɨ��ģʽ */
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;                      /* ��������ת�� */
    g_adc_dma_handle.Init.NbrOfConversion = ADC1_CH_NUM;                    /* ��ʵ���õ�1������ͨ������ */
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

    /* ����DMA�����������ж����ȼ� */
    HAL_NVIC_SetPriority(ADC_ADC1_DMASx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(ADC_ADC1_DMASx_IRQn);

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
 * @brief       ʹ��һ��ADC DMA����
 * @param       cndtr: DMA����Ĵ���
 * @retval      ��
 */
void adc3_dma_enable(uint16_t cndtr)
{
    __HAL_ADC_DISABLE(&g_adc3_dma_handle);           /* �ȹر�ADC */

    __HAL_DMA_DISABLE(&g_dma_adc3_handle);           /* �ر�DMA���� */
    g_dma_adc3_handle.Instance->NDTR = cndtr;        /* ����DMA���������� */
    __HAL_DMA_ENABLE(&g_dma_adc3_handle);            /* ����DMA���� */

    __HAL_ADC_ENABLE(&g_adc3_dma_handle);            /* ��������ADC */
    ADC_ADC3->CR2 |= 1 << 30;                       /* ��������ת��ͨ�� */
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void ADC_ADC1_DMASx_IRQHandler(void)
{
    if (ADC_ADCX_DMASx_IS_TC())
    {
        g_adc_dma_sta = 1;                          /* ���DMA������� */
        ADC_ADCX_DMASx_CLR_TC();                    /* ���DMA2 ������4 ��������ж� */
    }
}


/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void ADC_ADC3_DMASx_IRQHandler(void)
{
    if (ADC_ADC3_DMASx_IS_TC())
    {
        g_adc3_dma_sta = 1;                          /* ���DMA������� */
        ADC_ADC3_DMASx_CLR_TC();                    /* ���DMA2 ������4 ��������ж� */
    }
}





