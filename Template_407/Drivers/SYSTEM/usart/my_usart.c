/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
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
 * V1.0 20211014
 * ��һ�η���
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
/* ���ʹ��os,����������ͷ�ļ����� */

/******************************************************************************************/
/* �������´���, ֧��printf���� */
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
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

#if USART_EN_RX                                     /* ���ʹ���˽��� */

/* ���ջ���, ���USART_REC_LEN���ֽ�. */
uint8_t g_usart_rx_buf[USART_REC_LEN];

/*  ����״̬
 *  bit15��      ������ɱ�־
 *  bit14��      ���յ�0x0d
 *  bit13~0��    ���յ�����Ч�ֽ���Ŀ
*/
uint16_t g_usart_rx_sta = 0;

uint8_t g_rx_buffer[RXBUFFERSIZE];                  /* HAL��ʹ�õĴ��ڽ��ջ��� */

UART_HandleTypeDef g_uart1_handle;                  /* UART��� */


/**
 * @brief       ����X��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @note        ע��: ����������ȷ��ʱ��Դ, ���򴮿ڲ����ʾͻ������쳣.
 *              �����USART��ʱ��Դ��sys_stm32_clock_init()�������Ѿ����ù���.
 * @retval      ��
 */
void usart_init(uint32_t baudrate)
{
    /* UART ��ʼ������ */
    g_uart1_handle.Instance = USART_UX;                         /* USART1 */
    g_uart1_handle.Init.BaudRate = baudrate;                    /* ������ */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;             /* һ��ֹͣλ */
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;              /* ����żУ��λ */
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* ��Ӳ������ */
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;                 /* �շ�ģʽ */
    HAL_UART_Init(&g_uart1_handle);                             /* HAL_UART_Init()��ʹ��UART1 */

    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
}

/**
 * @brief       UART�ײ��ʼ������
 * @param       huart: UART�������ָ��
 * @note        �˺����ᱻHAL_UART_Init()����
 *              ���ʱ��ʹ�ܣ��������ã��ж�����
 * @retval      ��
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;
    if(huart->Instance == USART1)                               /* ����Ǵ���1�����д���1 MSP��ʼ�� */
    {
        USART_UX_CLK_ENABLE();                                  /* USART1 ʱ��ʹ�� */
        USART_TX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */
        USART_RX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* TX���� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
        gpio_init_struct.Alternate = USART_TX_GPIO_AF;          /* ����ΪUSART1 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */

        gpio_init_struct.Pin = USART_RX_GPIO_PIN;               /* RX���� */
        gpio_init_struct.Alternate = USART_RX_GPIO_AF;          /* ����ΪUSART1 */
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */
        //__HAL_UART_DISABLE_IT(&g_uart1_handle, UART_IT_TC);     /* �رշ����ж� */
#if USART_EN_RX


        //__HAL_UART_ENABLE_IT(&g_uart1_handle, UART_IT_RXNE);    /* ���������ж� */
        HAL_NVIC_EnableIRQ(USART_UX_IRQn);                      /* ʹ��USART1�ж�ͨ�� */
        HAL_NVIC_SetPriority(USART_UX_IRQn, 8, 0);              /* ��ռ���ȼ�8�������ȼ�0 */
        //printf("Set usart priority to %d\r\n", NVIC_EncodePriority(NVIC_PRIORITYGROUP_3, 3, 1));

#endif
    }
}



/**
 * @brief       �������ݽ��ջص�����
                ���ݴ������������,���ж�����Ϣ���з�������
 * @param       huart:���ھ��
 * @retval      ��
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// We have not woken a task at the start of the ISR.
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
    if(huart->Instance == USART1)               /* ����Ǵ���1 */
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
 * @brief       ����1�жϷ�����
 * @param       ��
 * @retval      ��
 */

void USART_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
        HAL_UART_Receive(&g_uart1_handle, &res, 1, 1000);
        //debug_handle(&res);
    }

    uint32_t timeout = 0;
    uint32_t maxDelay = 0x1FFFF;

    HAL_UART_IRQHandler(&g_uart1_handle);       /* ����HAL���жϴ����ú��� */

    timeout = 0;
    while (HAL_UART_GetState(&g_uart1_handle) != HAL_UART_STATE_READY) /* �ȴ����� */
    {
        timeout++;                              /* ��ʱ���� */
        if(timeout > maxDelay)
        {
            break;
        }
    }

    timeout=0;
    /* һ�δ������֮�����¿����жϲ�����RxXferCountΪ1 */
    while (HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE) != HAL_OK)
    {
        timeout++;                              /* ��ʱ���� */
        if (timeout > maxDelay)
        {
            break;
        }
    }
}

#endif









