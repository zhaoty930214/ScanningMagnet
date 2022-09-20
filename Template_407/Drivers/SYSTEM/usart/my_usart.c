/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       串口初始化代码(一般是串口1)，支持printf
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
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include <my_usart.h>
#include "sys.h"
//#include "uart_commu.h"
#include "freertos.h"
//#include "portmacro.h"
#include "queue.h"
#include "tsk_functions.h"
#include "rs232.h"
#include "main.h"
/* 如果使用os,则包括下面的头文件即可 */

/******************************************************************************************/
/* 加入以下代码, 支持printf函数 */
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE  int __io_putchar(int ch)

#else
#define PUTCHAR_PROTOTYPE  int fgetc(FILE * f)

#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

/***********************************************END*******************************************/

#if USART_EN_RX                                     /* 如果使能了接收 */

/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t g_usart_rx_buf[USART_REC_LEN];

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
 *  bit13~0，    接收到的有效字节数目
*/
uint16_t g_usart_rx_sta = 0;

uint8_t g_rx_buffer[RXBUFFERSIZE];                  /* HAL库使用的串口接收缓冲 */

UART_HandleTypeDef g_uart1_handle;                  /* UART句柄 */


/**
 * @brief       串口X初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart_init(uint32_t baudrate)
{
    /* UART 初始化设置 */
    g_uart1_handle.Instance = USART_UX;                         /* USART1 */
    g_uart1_handle.Init.BaudRate = baudrate;                    /* 波特率 */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* 字长为8位数据格式 */
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;             /* 一个停止位 */
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;              /* 无奇偶校验位 */
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* 无硬件流控 */
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;                 /* 收发模式 */
    HAL_UART_Init(&g_uart1_handle);                             /* HAL_UART_Init()会使能UART1 */

    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
}

/**
 * @brief       UART底层初始化函数
 * @param       huart: UART句柄类型指针
 * @note        此函数会被HAL_UART_Init()调用
 *              完成时钟使能，引脚配置，中断配置
 * @retval      无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;
    if(huart->Instance == USART1)                               /* 如果是串口1，进行串口1 MSP初始化 */
    {
        USART_UX_CLK_ENABLE();                                  /* USART1 时钟使能 */
        USART_TX_GPIO_CLK_ENABLE();                             /* 发送引脚时钟使能 */
        USART_RX_GPIO_CLK_ENABLE();                             /* 接收引脚时钟使能 */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* TX引脚 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
        gpio_init_struct.Alternate = USART_TX_GPIO_AF;          /* 复用为USART1 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* 初始化发送引脚 */

        gpio_init_struct.Pin = USART_RX_GPIO_PIN;               /* RX引脚 */
        gpio_init_struct.Alternate = USART_RX_GPIO_AF;          /* 复用为USART1 */
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* 初始化接收引脚 */
        //__HAL_UART_DISABLE_IT(&g_uart1_handle, UART_IT_TC);     /* 关闭发送中断 */
#if USART_EN_RX


        //__HAL_UART_ENABLE_IT(&g_uart1_handle, UART_IT_RXNE);    /* 开启接收中断 */
        HAL_NVIC_EnableIRQ(USART_UX_IRQn);                      /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority(USART_UX_IRQn, 8, 0);              /* 抢占优先级8，子优先级0 */
        //printf("Set usart priority to %d\r\n", NVIC_EncodePriority(NVIC_PRIORITYGROUP_3, 3, 1));

#endif
    }
}



/**
 * @brief       串口数据接收回调函数
                数据处理在这里进行,从中断向消息队列发送内容
 * @param       huart:串口句柄
 * @retval      无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// We have not woken a task at the start of the ISR.
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
    if(huart->Instance == USART1)               /* 如果是串口1 */
    {
#ifdef DEBUG_ON_USART1
		xQueueSendFromISR(Queue_Usart, &g_rx_buffer[0], &xHigherPriorityTaskWoken);

		if(pdTRUE == xHigherPriorityTaskWoken)
		{
			// Actual macro used here is port specific.
			portYIELD_FROM_ISR(pdTRUE);
		}
#endif
    }
//    else if(huart->Instance == RS232_UX)
//    {
//    	printf("Uart5 interrupt\r\n");
//		xQueueSendFromISR(Queue_Usart, &g_RS232_rx_buf[0], &xHigherPriorityTaskWoken);
//
//		if(pdTRUE == xHigherPriorityTaskWoken)
//		{
//			// Actual macro used here is port specific.
//			portYIELD_FROM_ISR(pdTRUE);
//		}
//    }
}

/**
 * @brief       串口1中断服务函数
 * @param       无
 * @retval      无
 */

void USART_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_RXNE) != RESET)) /* 接收到数据 */
    {
        HAL_UART_Receive(&g_uart1_handle, &res, 1, 1000);
        //debug_handle(&res);
    }

    uint32_t timeout = 0;
    uint32_t maxDelay = 0x1FFFF;

    HAL_UART_IRQHandler(&g_uart1_handle);       /* 调用HAL库中断处理公用函数 */

    timeout = 0;
    while (HAL_UART_GetState(&g_uart1_handle) != HAL_UART_STATE_READY) /* 等待就绪 */
    {
        timeout++;                              /* 超时处理 */
        if(timeout > maxDelay)
        {
            break;
        }
    }

    timeout=0;
    /* 一次处理完成之后，重新开启中断并设置RxXferCount为1 */
    while (HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE) != HAL_OK)
    {
        timeout++;                              /* 超时处理 */
        if (timeout > maxDelay)
        {
            break;
        }
    }
}

#endif









