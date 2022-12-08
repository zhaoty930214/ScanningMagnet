/**
 ****************************************************************************************************
 * @file        adc.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-18
 * @brief       ADC 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com/forum.php
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211018
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "adc.h"
#include "delay.h"


DMA_HandleTypeDef g_dma_adc_handle = {0};                                   /* 定义要搬运ADC数据的DMA句柄 */
ADC_HandleTypeDef g_adc_dma_handle = {0};                                   /* 定义ADC（DMA读取）句柄 */
uint8_t g_adc_dma_sta = 0;                                                  /* DMA传输状态标志, 0,未完成; 1, 已完成 */

uint8_t g_adc3_dma_sta = 0;
DMA_HandleTypeDef g_dma_adc3_handle = {0};
ADC_HandleTypeDef g_adc3_dma_handle = {0};                                   /* 定义ADC（DMA读取）句柄 */

/**
 * @brief       ADC 实例对象2 DMA读取 初始化函数.使能多通道连续采样模式+扫描模式+DMA传输模式项目中一共使用7个ADC通道，其中4个来自ADC
 * 				实例1(ADC1),3个来自ADC实例2(ADC3)。ADC1使用DMA2_Stream4/ADC3使用DMA2_Stream0，具体参考https://mevi
 * 				on.atlassian.net/wiki/spaces/YBL/pages/59506920/ADC1+ADC3+DMA
 * @param       mar: 存储器地址，即从ADC外设读回的数据要存储到的缓存的起始地址
 * @retval      无
 * @Version		作者							日期				修改目的
 * 1.0			tianyu.zhao@mevion.com		2022-11-16		初版
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
    __HAL_RCC_DMA2_CLK_ENABLE();                                        /* DMA2时钟使能 */

    gpio_init_struct.Pin = ADC_ADC3_CH5_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
	gpio_init_struct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(ADC_ADC3_CH5_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = ADC_ADC3_CH6_GPIO_PIN;
	HAL_GPIO_Init(ADC_ADC3_CH6_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = ADC_ADC3_CH7_GPIO_PIN;
	HAL_GPIO_Init(ADC_ADC3_CH7_GPIO_PORT, &gpio_init_struct);

	/* 初始化DMA */
	g_dma_adc3_handle.Instance = 				 	ADC3_DMASx;
	g_dma_adc3_handle.Init.Channel = 				ADC_ADC3_DMASx_Chanel;                /* 设置DMA通道 */
	g_dma_adc3_handle.Init.Direction = 				DMA_PERIPH_TO_MEMORY;                 /* 从外设到存储器模式 */
	g_dma_adc3_handle.Init.PeriphInc = 				DMA_PINC_DISABLE;                     /* 外设非增量模式 */
	g_dma_adc3_handle.Init.MemInc = 				DMA_MINC_ENABLE;                      /* 存储器增量模式 */
    g_dma_adc3_handle.Init.PeriphDataAlignment = 	DMA_PDATAALIGN_HALFWORD;    		  /* 外设数据长度:16位 */
    g_dma_adc3_handle.Init.MemDataAlignment = 		DMA_MDATAALIGN_HALFWORD;       		  /* 存储器数据长度:16位 */
    g_dma_adc3_handle.Init.Mode = 					DMA_NORMAL;                           /* 外设流控模式 */
    g_dma_adc3_handle.Init.Priority =		 		DMA_PRIORITY_MEDIUM;                  /* 中等优先级 */
    HAL_DMA_Init(&g_dma_adc3_handle);


    g_adc3_dma_handle.Instance = 					ADC_ADC3;
    g_adc3_dma_handle.Init.ClockPrescaler = 		ADC_CLOCKPRESCALER_PCLK_DIV4;    	  /* 4分频，21Mhz */
    g_adc3_dma_handle.Init.Resolution = 			ADC_RESOLUTION_12B;                   /* 12位模式 */
    g_adc3_dma_handle.Init.DataAlign = 				ADC_DATAALIGN_RIGHT;                  /* 右对齐 */
    g_adc3_dma_handle.Init.ScanConvMode = 			ENABLE;                            	  /* 非扫描模式 */
    g_adc3_dma_handle.Init.ContinuousConvMode =	 	ENABLE;                      		  /* 开启连续转换 */
    g_adc3_dma_handle.Init.NbrOfConversion = 		3;                    				  /* 本实验用到3个规则通道序列 */
    g_adc3_dma_handle.Init.DiscontinuousConvMode = 	DISABLE;                  			  /* 禁止不连续采样模式 */
    g_adc3_dma_handle.Init.NbrOfDiscConversion = 	0;                          		  /* 不连续采样通道数为0 */
    g_adc3_dma_handle.Init.ExternalTrigConv = 		ADC_SOFTWARE_START;            		  /* 软件触发 */
    g_adc3_dma_handle.Init.DMAContinuousRequests = 	ENABLE;                   			  /* 开启DMA请求 */
    HAL_ADC_Init(&g_adc3_dma_handle);                                        			  /* 初始化ADC */

    __HAL_LINKDMA(&g_adc3_dma_handle, DMA_Handle, g_dma_adc3_handle);         			  /* 把ADC和DMA连接起来 */

    /* *************配置ADC通道, 前四个通道是外设ADC1的通道；后三个通道是外设ADC3的通道**************** */
    adc_ch_conf.Channel = ADC_ADCX_CH5;                                     /* 通道 */
    adc_ch_conf.Rank = 1;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc3_dma_handle, &adc_ch_conf);                /* 通道配置 */

    adc_ch_conf.Channel = ADC_ADCX_CH6;                                     /* 通道 */
    adc_ch_conf.Rank = 2;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc3_dma_handle, &adc_ch_conf);                /* 通道配置 */

    adc_ch_conf.Channel = ADC_ADCX_CH7;                                     /* 通道 */
    adc_ch_conf.Rank = 3;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc3_dma_handle, &adc_ch_conf);                /* 通道配置 */

    /* *************配置ADC通道, 前四个通道是外设ADC1的通道；后三个通道是外设ADC3的通道**************** */


    /* 配置DMA数据流请求中断优先级 */
    HAL_NVIC_SetPriority(ADC_ADC3_DMASx_IRQn, 3, 4);
    HAL_NVIC_EnableIRQ(ADC_ADC3_DMASx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc3_handle, (uint32_t)&ADC3->DR, mar, 0);       /* 启动DMA，并开启中断 */
    HAL_ADC_Start_DMA(&g_adc3_dma_handle,&mar,0);
}

/**
 * @brief       ADC 实例对象1 DMA读取 初始化函数.使能多通道连续采样模式+扫描模式+DMA传输模式项目中一共使用7个ADC通道，其中4个来自ADC
 * 				实例1(ADC1),3个来自ADC实例2(ADC3)。ADC1使用DMA2_Stream4/ADC3使用DMA2_Stream0，具体参考https://mevi
 * 				on.atlassian.net/wiki/spaces/YBL/pages/59506920/ADC1+ADC3+DMA
 * @param       mar: 存储器地址，即从ADC外设读回的数据要存储到的缓存的起始地址
 * @retval      无
 * @Version		作者							日期				修改目的
 * 1.0			tianyu.zhao@mevion.com		2022-11-16		初版
 * 1.1			tianyu.zhao@mevion.com		2022-11-07		https://mevion.atlassian.net/browse/YBL-948
 */
void adc_instance1_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    ADC_ADCX_CHY_CLK_ENABLE();                                              /* 使能ADCx时钟 */
    ADC_ADCX_CH1_GPIO_CLK_ENABLE();                                         /* 开启GPIO时钟 */
    ADC_ADCX_CH2_GPIO_CLK_ENABLE();                                         /* 开启GPIO时钟 */
    ADC_ADCX_CH3_GPIO_CLK_ENABLE();                                         /* 开启GPIO时钟 */
    ADC_ADCX_CH4_GPIO_CLK_ENABLE();                                         /* 开启GPIO时钟 */

    if ((uint32_t)ADC1_DMASx > (uint32_t)DMA2)                          /* 大于DMA2的基地址, 则为DMA2的数据流通道了 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                                        /* DMA2时钟使能 */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                                        /* DMA1时钟使能 */
    }

    /* 设置AD采集通道对应IO引脚工作模式 */
    gpio_init_struct.Pin = ADC_ADCX_CH1_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ADC_ADCX_CH1_GPIO_PORT, &gpio_init_struct);

    /* 设置AD采集通道对应IO引脚工作模式 */
    gpio_init_struct.Pin = ADC_ADCX_CH2_GPIO_PIN;
    HAL_GPIO_Init(ADC_ADCX_CH2_GPIO_PORT, &gpio_init_struct);

    /* 设置AD采集通道对应IO引脚工作模式 */
    gpio_init_struct.Pin = ADC_ADCX_CH3_GPIO_PIN;
    HAL_GPIO_Init(ADC_ADCX_CH3_GPIO_PORT, &gpio_init_struct);

    /* 设置AD采集通道对应IO引脚工作模式 */
    gpio_init_struct.Pin = ADC_ADCX_CH4_GPIO_PIN;
    HAL_GPIO_Init(ADC_ADCX_CH4_GPIO_PORT, &gpio_init_struct);


    /* 初始化DMA */
    g_dma_adc_handle.Instance = ADC1_DMASx;                             /* 设置DMA数据流 */
    g_dma_adc_handle.Init.Channel = DMA_CHANNEL_0;                          /* 设置DMA通道 */
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;                 /* 从外设到存储器模式 */
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                     /* 外设非增量模式 */
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                         /* 存储器增量模式 */
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* 外设数据长度:16位 */
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* 存储器数据长度:16位 */
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                                /* 外设流控模式 */
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* 中等优先级 */
    HAL_DMA_Init(&g_dma_adc_handle);

    g_adc_dma_handle.Instance = ADC_ADC1;
    g_adc_dma_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;    /* 4分频，21Mhz */
    g_adc_dma_handle.Init.Resolution = ADC_RESOLUTION_12B;                  /* 12位模式 */
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                  /* 右对齐 */
    g_adc_dma_handle.Init.ScanConvMode = ENABLE;                            /* 扫描模式 */
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;                      /* 开启连续转换 */
    g_adc_dma_handle.Init.NbrOfConversion = ADC1_CH_NUM;                    /* 本实验用到1个规则通道序列 */
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;                  /* 禁止不连续采样模式 */
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;                          /* 不连续采样通道数为0 */
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;            /* 软件触发 */
    g_adc_dma_handle.Init.DMAContinuousRequests = ENABLE;                   /* 开启DMA请求 */
    HAL_ADC_Init(&g_adc_dma_handle);                                        /* 初始化ADC */

    __HAL_LINKDMA(&g_adc_dma_handle, DMA_Handle, g_dma_adc_handle);         /* 把ADC和DMA连接起来 */

    /* *************配置ADC通道, 前四个通道是外设ADC1的通道；后三个通道是外设ADC3的通道**************** */
    adc_ch_conf.Channel = ADC_ADCX_CH1;                                     /* 通道 */
    adc_ch_conf.Rank = 1;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* 通道配置 */

    adc_ch_conf.Channel = ADC_ADCX_CH2;                                     /* 通道 */
    adc_ch_conf.Rank = 2;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* 通道配置 */

    adc_ch_conf.Channel = ADC_ADCX_CH3;                                     /* 通道 */
    adc_ch_conf.Rank = 3;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* 通道配置 */


    adc_ch_conf.Channel = ADC_ADCX_CH4;                                     /* 通道 */
    adc_ch_conf.Rank = 4;                                                   /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);                 /* 通道配置 */
    /* *************配置ADC通道, 前四个通道是外设ADC1的通道；后三个通道是外设ADC3的通道**************** */

    /* 配置DMA数据流请求中断优先级 */
    HAL_NVIC_SetPriority(ADC_ADC1_DMASx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(ADC_ADC1_DMASx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0);       /* 启动DMA，并开启中断 */
    HAL_ADC_Start_DMA(&g_adc_dma_handle,&mar,0);                            /* 开启ADC，通过DMA传输结果 */
}



/**
 * @brief       使能一次ADC DMA传输
 * @param       cndtr: DMA传输的次数
 * @retval      无
 */
void adc_dma_enable(uint16_t cndtr)
{
    __HAL_ADC_DISABLE(&g_adc_dma_handle);           /* 先关闭ADC */

    __HAL_DMA_DISABLE(&g_dma_adc_handle);           /* 关闭DMA传输 */
    g_dma_adc_handle.Instance->NDTR = cndtr;        /* 重设DMA传输数据量 */
    __HAL_DMA_ENABLE(&g_dma_adc_handle);            /* 开启DMA传输 */

    __HAL_ADC_ENABLE(&g_adc_dma_handle);            /* 重新启动ADC */
    ADC_ADC1->CR2 |= 1 << 30;                       /* 启动规则转换通道 */
}

/**
 * @brief       使能一次ADC DMA传输
 * @param       cndtr: DMA传输的次数
 * @retval      无
 */
void adc3_dma_enable(uint16_t cndtr)
{
    __HAL_ADC_DISABLE(&g_adc3_dma_handle);           /* 先关闭ADC */

    __HAL_DMA_DISABLE(&g_dma_adc3_handle);           /* 关闭DMA传输 */
    g_dma_adc3_handle.Instance->NDTR = cndtr;        /* 重设DMA传输数据量 */
    __HAL_DMA_ENABLE(&g_dma_adc3_handle);            /* 开启DMA传输 */

    __HAL_ADC_ENABLE(&g_adc3_dma_handle);            /* 重新启动ADC */
    ADC_ADC3->CR2 |= 1 << 30;                       /* 启动规则转换通道 */
}

/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void ADC_ADC1_DMASx_IRQHandler(void)
{
    if (ADC_ADCX_DMASx_IS_TC())
    {
        g_adc_dma_sta = 1;                          /* 标记DMA传输完成 */
        ADC_ADCX_DMASx_CLR_TC();                    /* 清除DMA2 数据流4 传输完成中断 */
    }
}


/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void ADC_ADC3_DMASx_IRQHandler(void)
{
    if (ADC_ADC3_DMASx_IS_TC())
    {
        g_adc3_dma_sta = 1;                          /* 标记DMA传输完成 */
        ADC_ADC3_DMASx_CLR_TC();                    /* 清除DMA2 数据流4 传输完成中断 */
    }
}





