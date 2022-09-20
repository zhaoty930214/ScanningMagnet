/**
 ****************************************************************************************************
 * @file        rs232.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 7* @date        2021-10-27
 * @brief       RS232 ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� F407���������
 * ������Ƶ��www.yuanzige.com
 * ������̳��http://www.openedv.com/forum.php
 * ��˾��ַ��www.alientek.com
 * �����ַ��zhengdianyuanzi.tmall.com
 *
 * �޸�˵��
 * V1.0 20211027
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "rs232.h"
#include "delay.h"
#include "freertos.h"
#include "queue.h"
#include "tsk_functions.h"
#include "my_usart.h"
#include "LED.h"
UART_HandleTypeDef g_rs232_handler;     /* RS232���ƾ��(����) */

#ifdef RS232_EN_RX /* ���ʹ���˽��� */

uint8_t g_RS232_rx_buf[RS232_REC_LEN]; /* ���ջ���, ��� RS232_REC_LEN ���ֽ�. */
uint8_t g_RS232_rx_cnt = 0;            /* ���յ������ݳ��� */

void RS232_UX_IRQHandler(void)
{
    uint8_t res;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
    if ((__HAL_UART_GET_FLAG(&g_rs232_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
#ifndef DEBUG_ON_USART1
        HAL_UART_Receive(&g_rs232_handler, &res, 1, 1000);
        LED1_TOGGLE();
		xQueueSendFromISR(Queue_Usart, &res, &xHigherPriorityTaskWoken);

		if(pdTRUE == xHigherPriorityTaskWoken)
		{
			// Actual macro used here is port specific.
			portYIELD_FROM_ISR(pdTRUE);
		}
#endif
//        if (g_RS232_rx_cnt < RS232_REC_LEN)         /* ������δ�� */
//        {
//            g_RS232_rx_buf[g_RS232_rx_cnt] = res;   /* ��¼���յ���ֵ */
//            g_RS232_rx_cnt++;                       /* ������������1 */
//        }
    }

//    uint8_t res;
//
//    LED1_TOGGLE();
//    printf("Uart5 interrupt\r\n");
//
//    if ((__HAL_UART_GET_FLAG(&g_rs232_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
//    {
//        HAL_UART_Receive(&g_rs232_handler, &res, 1, 1000);
//    }
//
//    uint32_t timeout = 0;
//    uint32_t maxDelay = 0x1FFFF;
//
//    HAL_UART_IRQHandler(&g_rs232_handler);       /* ����HAL���жϴ����ú��� */
//
//    timeout = 0;
//    while (HAL_UART_GetState(&g_rs232_handler) != HAL_UART_STATE_READY) /* �ȴ����� */
//    {
//        timeout++;                              /* ��ʱ���� */
//        if(timeout > maxDelay)
//        {
//            break;
//        }
//    }
//
//    timeout=0;
//    /* һ�δ������֮�����¿����жϲ�����RxXferCountΪ1 */
//    while (HAL_UART_Receive_IT(&g_rs232_handler, (uint8_t *)g_RS232_rx_buf, RXBUFFERSIZE) != HAL_OK)
//    {
//        timeout++;                              /* ��ʱ���� */
//        if (timeout > maxDelay)
//        {
//            break;
//        }
//    }
}

#endif

/**
 * @brief       RS232��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void rs232_init(uint32_t baudrate)
{

    /* IO �� ʱ������ */
    RS232_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    RS232_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    RS232_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_init_struct;
    
    gpio_init_struct.Pin = RS232_TX_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = RS232_AF_UART5;                /* ����Ϊ����5 */
    HAL_GPIO_Init(RS232_TX_GPIO_PORT, &gpio_init_struct);       /* ����TX �� ģʽ���� */

    gpio_init_struct.Pin = RS232_RX_GPIO_PIN;
    //gpio_init_struct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(RS232_RX_GPIO_PORT, &gpio_init_struct);       /* ����RX �� �������ó�����ģʽ */

    /* USART ��ʼ������ */
    g_rs232_handler.Instance = RS232_UX;                    /* ѡ��232��Ӧ�Ĵ��� */
    g_rs232_handler.Init.BaudRate = baudrate;               /* ������ */
    g_rs232_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_rs232_handler.Init.StopBits = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_rs232_handler.Init.Parity = UART_PARITY_NONE;         /* ����żУ��λ */
    g_rs232_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* ��Ӳ������ */
    g_rs232_handler.Init.Mode = UART_MODE_TX_RX;            /* �շ�ģʽ */
    HAL_UART_Init(&g_rs232_handler);                        /* ʹ�ܶ�Ӧ�Ĵ��� */
    __HAL_UART_DISABLE_IT(&g_rs232_handler, UART_IT_TC);

#if RS232_EN_RX /* ���ʹ���˽��� */
    __HAL_UART_ENABLE_IT(&g_rs232_handler, UART_IT_RXNE);   /* ���������ж� */
    HAL_NVIC_EnableIRQ(RS232_UX_IRQn);                      /* ʹ��RS232�ж� */
    HAL_NVIC_SetPriority(RS232_UX_IRQn, 8, 0);              /* ��ռ���ȼ�3, �����ȼ�3 */
#endif

}

/**
 * @brief       RS232����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� RS232_REC_LEN ���ֽ�)
 * @retval      ��
 */
void rs232_send_data(uint8_t *buf, uint8_t len)
{
    HAL_UART_Transmit(&g_rs232_handler, buf, len, 1000); /* RS232�������� */
    g_RS232_rx_cnt = 0;
}

/**
 * @brief       RS232��ѯ���յ�������
 * @param       buf     : ���ջ������׵�ַ
 * @param       len     : ���յ������ݳ���
 *   @arg               0   , ��ʾû�н��յ��κ�����
 *   @arg               ����, ��ʾ���յ������ݳ���
 * @retval      ��
 */
void rs232_receive_data(uint8_t *buf, uint8_t *len)
{
    uint8_t rxlen = g_RS232_rx_cnt;
    uint8_t i = 0;
    *len = 0;     /* Ĭ��Ϊ0 */
    delay_ms(10); /* �ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս��� */

    if (rxlen == g_RS232_rx_cnt && rxlen) /* ���յ�������,�ҽ�������� */
    {
        for (i = 0; i < rxlen; i++)
        {
            buf[i] = g_RS232_rx_buf[i];
        }

        *len = g_RS232_rx_cnt; /* ��¼�������ݳ��� */
        g_RS232_rx_cnt = 0;    /* ���� */
    }
}
