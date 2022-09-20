/*
 * my_tasks.c
 *
 *  Created on: Aug 23, 2022
 *      Author: tianyu.zhao
 */
#include <stdio.h>
#include <stdlib.h>

#include "my_tasks.h"
#include "LED.h"
#include "uart_commu.h"
#include "timers.h"
#include "Measure_Types.h"
#include "gtim.h"
#include "pid.h"
#include "stepper_tim.h"
#include "stepper_motor.h"
#include "Measure_Constants.h"
#include "main.h"
#include "tsk_functions.h"
#include "adc.h"

/*vTastList������*/
char pcWriteBuffer[2048];

/*��������ȫ�ֽṹ�����*/
MeasureConfig_t g_measureCfg;

/**
 * ������
 */
TaskHandle_t	Statistic_Handle_t;
TaskHandle_t 	HeartBeat_Handle_t;
TaskHandle_t	Usart_Handle_t;
TaskHandle_t	Measure_Handle_t;

void AutoReloadCallback(TimerHandle_t xTimer);





/**
 * ͳ�������ջ�ռ�����
 */
void Statistic_task(void *arg)
{
#if ENABLE_TRACE
	int encoder_cnt;
	while(1)
	{

		taskENTER_CRITICAL();

		vTaskList((char *)&pcWriteBuffer);
		printf("task_name   task_state  priority   stack  task_num\r\n");
		printf("%s\r\n", pcWriteBuffer);

		size_t free_heap = xPortGetFreeHeapSize();	         //��ȡ��ǰδ������ڴ�Ѵ�С
		size_t minimum_heap = xPortGetMinimumEverFreeHeapSize();	 //��ȡδ������ڴ����ʷ��Сֵ

		printf("Current FreeHeapSize is %d\r\n", free_heap);
		printf("Minimum EverFreeHeapSize is %d\r\n", minimum_heap);

		encoder_cnt = gtim_get_encode();
		printf("encoder value now is %d\r\n", encoder_cnt);

		taskEXIT_CRITICAL();


		vTaskDelay(5*1000);

	}
#endif
}

extern int8_t g_adc_dma_sta;

#define ADC_BUF_LEN 100
uint16_t adc_buff[ADC_BUF_LEN];


/**
 * ��������LED������˸��ָʾϵͳ����״̬
 */
void HeartBeat_task(void* arg)
{
	uint8_t cnt  = 0;
	uint16_t adcx;

	uint32_t sum;

	float temp;
	int i;

	adc_dma_init((uint32_t)&adc_buff[0]);
	adc_dma_enable(ADC_BUF_LEN);
	init_input_io();
	while(1)
	{
		if(cnt++%10 == 0)
		{
			LED0_TOGGLE();
//			if(g_adc_dma_sta == 1)
//			{
//	            /* ����DMA �ɼ�����ADC���ݵ�ƽ��ֵ */
//	            sum = 0;
//	            for (i = 0; i < ADC_BUF_LEN; i++)              /* �ۼ� */
//	            {
//	                sum += adc_buff[i];
//	            }
//
//	            adcx = sum / ADC_BUF_LEN;                      /* ȡƽ��ֵ */
//
//
//	            temp = (float)adcx * (3.3 / 4096);             /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//
//				printf("ADC sample value is %f\r\n", temp);

//	            g_adc_dma_sta = 0;                             /* ���DMA�ɼ����״̬��־ */
//	            adc_dma_enable(ADC_BUF_LEN);                   /* ������һ��ADC DMA�ɼ� */

//	            adcx = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
//	            printf("PA15 Input value is %d\r\n", adcx);

			taskENTER_CRITICAL();
			i = gtim_get_encode();
			printf("Encoder cnt is %d\r\n", i);
			printf("\033[1A"); //�Ȼص���һ��
			taskEXIT_CRITICAL();
//			}

		}
		vTaskDelay(30);
	}
}




/**
 * �������ݴ�������,��������ͬ��ָ�ִ�в�ͬ�Ķ���
 */
void UsartCommu_task(void *arg)
{
	uint8_t queue_uart_item;
	uint8_t cmd_channel;
	uint8_t measure_id;
	uint32_t notify_value;
	//portBASE_TYPE	xStatus;

	BaseType_t ret;
	while(1)
	{
		ret = xQueueReceive(Queue_Usart, &queue_uart_item, 100);
		if(ret == pdPASS)
		{
			if(debug_handle(&queue_uart_item, &g_measureCfg))
			{
				/*Toggle LED1 for indicate*/
				xTimerStart(AutoReloadTimer_Handle, 0);

				/*Send queue item to Measure_task*/
				cmd_channel = g_measureCfg.cmd_channel;

				switch(cmd_channel)
				{
				//��ʼ����ͨ��
				case 0xa2:
		    		measure_id = START_MEASURE;
		    		printf("%2.2f\t %2.2f\t %2.2f\t %2.2f\t \r\n",	\
		    				    				g_measureCfg.step_x,
		    									g_measureCfg.step_y,
		    									g_measureCfg.step_z,
		    									g_measureCfg.amplifier_current);

					xQueueSend(Queue_Measure, &measure_id, 1);
					break;

				//ֹͣ����ͨ��
				case 0xa3:
					//measure_id = STOP_MEASURE;
					//xQueueSend(Queue_Measure, &measure_id, 1);
					notify_value = NOTIFY_STOP_MEASURE;
					xTaskNotify(Measure_Handle_t, notify_value, eSetValueWithOverwrite);
					break;

				//��ȡ����
				case 0xa1:
					printf("Got ReadConfig msg\r\n");
					break;

				//��������
				case 0xa0:
					printf("Got SendConfig msg\r\n");
		    		printf("%2.2f\t %2.2f\t %2.2f\t %2.2f\t \r\n	\
		    				dirX: %d \r\n",
		    				g_measureCfg.step_x,
							g_measureCfg.step_y,
							g_measureCfg.step_z,
							g_measureCfg.amplifier_current,
							g_measureCfg.dir_x
							);

		    		update_stepper_params();
					break;

				case 0xa4:
		    		measure_id = DEBUG_MOVE;
		    		printf("%2.2f\t %2.2f\t %2.2f\t %2.2f\t \r\n",	\
		    				    				    				g_measureCfg.step_x,
		    				    									g_measureCfg.step_y,
		    				    									g_measureCfg.step_z,
		    				    									g_measureCfg.amplifier_current);
					xQueueSend(Queue_Measure, &measure_id, 1);
					break;
				}
			}
		}
		//vTaskDelay(5);
	}
}




/**
 * ���������漰���������������������PIDλ�û����˶�·���滮��
 */
void Measure_task(void *arg)
{
	uint8_t queue_recv_item;
	BaseType_t ret;
	printf("Start Measuring Task\r\n");
	for(;;)
	{
		ret = xQueueReceive(Queue_Measure, &queue_recv_item, 10);
		if( ret != pdTRUE)
		{
			continue;
		}
		else
		{
			ret = 1;
		}

		switch(queue_recv_item)
		{
			case START_MEASURE:
				tsk_Move_Execute(&ret);
				break;

			case STOP_MEASURE:

				break;

			case DEBUG_MOVE:
				sliede_way_test(g_measureCfg.step_x, g_measureCfg.dir_x);
				break;
		}
		vTaskDelay(1000);
	}
}


void start_task(void *pvParameter)
{
	taskENTER_CRITICAL();

	BaseType_t ret_val = pdFALSE;

	tsk_init_queues();
//	//����������ڶ�ʱ��, ���ڶ�ʱ��������100ms(100 ��ʱ�ӽ���),����ģʽ
//	AutoReloadTimer_Handle=xTimerCreate((const char*            )"AutoReloadTimer",
//										(TickType_t             )100,
//										(UBaseType_t            )pdTRUE,
//										(void*)1,
//										(TimerCallbackFunction_t)AutoReloadCallback);
//
//	//������Ϣ����
//	Queue_Usart = xQueueCreate(QUEUE_LENGTH, QUEUE_SIZE);
//	if(Queue_Usart == NULL)
//	{
//		printf("Create Queue_Usart failed\r\n");
//	}
//
//	Queue_Measure = xQueueCreate(Config_QUEUE_LENGTH, sizeof(MeasureConfig_t));
//	if(Queue_Measure == NULL)
//	{
//		printf("Create Queue_Measure failed\r\n");
//	}
//
//	/*�����λ��ֻ����д��1�����ȣ�X&&Y&&Z����λʱ���ܴ�PID�ж�������Ϣ����д��*/
//	Queue_MotorReady = xQueueCreate(1, QUEUE_SIZE);
//	if(Queue_MotorReady == NULL)
//	{
//		printf("Create Queue_MotorReady\r\n");
//	}

	init_measureCfg(0.2, 0.2, 0.2);
	//��������
#if ENABLE_TRACE
	ret_val = xTaskCreate(Statistic_task,
						STATISTIC_TASK_NAME,
						Statistic_Task_Stack,
						NULL,
						Statistic_Task_Prio,
						&Statistic_Handle_t);
	if( pdTRUE != ret_val){
		printf("Create Task %s failed\r\n", STATISTIC_TASK_NAME);
	}
#endif
	//������������
	ret_val = xTaskCreate(Measure_task,
						  Measure_TASK_NAME,
						  Measure_Task_Stack,
						  NULL,
						  Measure_Task_Prio,
						  &Measure_Handle_t);
	if( pdTRUE != ret_val){
	printf("Create Task %s failed\r\n", Measure_TASK_NAME);
	}

	//������������
	ret_val  = xTaskCreate(HeartBeat_task,
						HEARTBEAT_TASK_NAME,
						HeartBeat_Task_Stack,
						NULL,
						HeartBeat_Task_Prio,
						&HeartBeat_Handle_t);
	if( pdTRUE != ret_val){
		printf("Create Task %s failed\r\n", HEARTBEAT_TASK_NAME);
	}

	//������������
	ret_val = xTaskCreate(UsartCommu_task,
						  USART_TASK_NAME,
						  Usart_Task_Stack,
						  NULL,
						  Usart_Task_Prio,
						  &Usart_Handle_t);
	if( pdTRUE != ret_val){
		printf("Create Task %s failed\r\n", USART_TASK_NAME);
	}

	printf("Create Tasks Success\r\n");
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}





