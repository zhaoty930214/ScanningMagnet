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
#include "photoelectric_switch.h"
#include "usbh_core.h"
#include "usbh_msc.h"


//#include "SGLog.h"

/*vTastList缓冲区*/
#if ENABLE_TRACE
	char pcWriteBuffer[2048];
#endif

/*测试配置全局结构体变量*/
MeasureConfig_t g_measureCfg;

/**
 * 任务句柄
 */
TaskHandle_t	Statistic_Handle_t;
TaskHandle_t 	HeartBeat_Handle_t;
TaskHandle_t	Usart_Handle_t;
TaskHandle_t	Measure_Handle_t;



void AutoReloadCallback(TimerHandle_t xTimer);





/**
 * 统计任务的栈空间余量
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

		size_t free_heap = xPortGetFreeHeapSize();	         //获取当前未分配的内存堆大小
		size_t minimum_heap = xPortGetMinimumEverFreeHeapSize();	 //获取未分配的内存堆历史最小值

		printf("Current FreeHeapSize is %d\r\n", free_heap);
		printf("Minimum EverFreeHeapSize is %d\r\n", minimum_heap);

		encoder_cnt = gtim_get_encode();
		printf("encoder value now is %d\r\n", encoder_cnt);

		taskEXIT_CRITICAL();


		vTaskDelay(5*1000);

	}
#endif
}


/**
 * 心跳任务，LED周期闪烁，指示系统运行状态
 */
void HeartBeat_task(void* arg)
{
	uint8_t cnt  = 0;

//	uint8_t key_optical[3];

	while(1)
	{
		if(cnt++%10 == 0)
		{
			LED0_TOGGLE();
//adcx = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);

		#if 0 //外部输入中断调试
			key_optical[0] = HAL_GPIO_ReadPin(OPTICAL_KEY_1_PORT, OPTICAL_KEY_1_PIN);
			key_optical[1] = HAL_GPIO_ReadPin(OPTICAL_KEY_2_PORT, OPTICAL_KEY_2_PIN);
			key_optical[2] = HAL_GPIO_ReadPin(OPTICAL_KEY_3_PORT, OPTICAL_KEY_3_PIN);
			printf("Current TTL1 is %d \t, TTL2 is %d \t, TTL3 is %d\t \r\n",
					key_optical[0], key_optical[1], key_optical[2]);

			key_optical[0] = HAL_GPIO_ReadPin(THERMAL_COUPLING_INTERLOCK_PORT, THERMAL_COUPLING_INTERLOCK_PIN);
			printf("Current interlock1 is %d\r\n", key_optical[0]);

			key_optical[1] = HAL_GPIO_ReadPin(TEMPER_FLOW_INTERLOCK_PORT, TEMPER_FLOW_INTERLOCK_PIN);
			printf("Current interlock2 is %d\r\n", key_optical[1]);
		#endif
			//taskENTER_CRITICAL();
			//i = gtim_get_encode();
			//printf("Encoder cnt is %d\r\n", i);
			//printf("\033[1A"); //先回到上一行
			//printf("\033[1A"); //先回到上一行
			//taskEXIT_CRITICAL();
//			}

		}

		vTaskDelay(30);
	}
}

//COUNT_UART_CHANNELS
const Uart_Struct_t g_uart_handle_struct[UART_COMMUNICATION_CHANNEL_COUNT]=
{
	//channel_id	, 		PACK_SIZE		  , 	pstorage					 		, handler
	{CMD_SEND_CONFIG, 		PACK_SIZE_CONFIG  , 	&g_measureCfg.measure_config		, config_parse_handler},
	{CMD_READ_CONFIG, 		PACK_SIZE_GENERAL , 	NULL						  		, NULL				},
	{CMD_SEND_START,  		PACK_SIZE_GENERAL , 	NULL						  		, NULL				},
	{CMD_SEND_STOP,   		PACK_SIZE_GENERAL , 	NULL						  		, NULL				},
	{CMD_SEND_DEBUG,  		PACK_SIZE_DEBUG   , 	&g_measureCfg.debug_config    		, debug_parse_handler },
	{CMD_SEND_TIME,   		PACK_SIZE_TIME	  , 	&g_measureCfg.time_config     		, time_parse_handler  },
	{CMD_SEND_CALIBRATION,  PACK_SIZE_CALIBRATION,	&g_measureCfg.calibration_config	, calibration_parse_handler}
};


/**
 * 串口数据处理任务,解析出不同的指令，执行不同的动作
 */
void UsartCommu_task(void *arg)
{
	uint8_t queue_uart_item;
	uint8_t cmd_channel;
	uint8_t measure_id;
	uint32_t notify_value;
	//portBASE_TYPE	xStatus;
	BaseType_t ret;

//	Register_Uart_handler(CMD_SEND_CONFIG, &g_measureCfg.measure_config, PACK_SIZE_CONFIG, config_parse_handler);	//4*4+7=23
//	Register_Uart_handler(0xa1, NULL, PACK_SIZE_GENERAL, NULL);							//0+7
//	Register_Uart_handler(0xa2, NULL, PACK_SIZE_GENERAL, NULL);
//	Register_Uart_handler(0xa3, NULL, PACK_SIZE_GENERAL, NULL);
//	Register_Uart_handler(0xa4, &g_measureCfg.debug_config, PACK_SIZE_DEBUG, debug_parse_handler);	//1+1+4+7 = 13
//	//Register_Uart_handler(0xa4, &g_measureCfg.debug_config, PACK_SIZE_TIME);	//1+1+4+7 = 13

	while(1)
	{
		ret = xQueueReceive(Queue_Usart, &queue_uart_item, 100);
		if(ret == pdPASS);
		else continue;

		{
			if(debug_handle(&queue_uart_item, &g_measureCfg))
			{
				/*Toggle LED1 for indicate*/
				xTimerStart(AutoReloadTimer_Handle, 0);

				/*Send queue item to Measure_task*/
				cmd_channel = g_measureCfg.cmd_channel;

				switch(cmd_channel)
				{
				//开始测量通道
				case CMD_SEND_START:
		    		measure_id = START_MEASURE;
		    		printf("%2.2f\t %2.2f\t %2.2f\t %2.2f\t \r\n",	\
		    				    				g_measureCfg.measure_config.step_x,
		    									g_measureCfg.measure_config.step_y,
		    									g_measureCfg.measure_config.step_z,
		    									g_measureCfg.measure_config.amplifier_current);

					xQueueSend(Queue_Measure, &measure_id, 1);
					break;

				//停止测量通道
				case CMD_SEND_STOP:
					measure_id = STOP_MEASURE;
					xQueueSend(Queue_Measure, &measure_id, 1);
					notify_value = NOTIFY_STOP_MEASURE;
					xTaskNotify(Measure_Handle_t, notify_value, eSetValueWithOverwrite);

					break;

				//读取配置
				case CMD_READ_CONFIG:
					printf("Got ReadConfig msg\r\n");
					break;

				//发送配置
				case CMD_SEND_CONFIG:
					printf("Got SendConfig msg\r\n");
		    		printf("%2.2f\t %2.2f\t %2.2f\t %2.2f\t\r\n",
		    				g_measureCfg.measure_config.step_x,
							g_measureCfg.measure_config.step_y,
							g_measureCfg.measure_config.step_z,
							g_measureCfg.measure_config.amplifier_current
							);

		    		update_stepper_params();
					break;

				case CMD_SEND_DEBUG:
					measure_id = DEBUG_MOVE;
		    		printf("%2.2f\t %2.2f\t %2.2f\t %2.2f\t \r\n",	\
							g_measureCfg.measure_config.step_x,
							g_measureCfg.measure_config.step_y,
							g_measureCfg.measure_config.step_z,
							g_measureCfg.measure_config.amplifier_current);
					xQueueSend(Queue_Measure, &measure_id, 1);
					break;
				}
			}
		}
		//vTaskDelay(5);
	}
}



/**
 * 测量任务，涉及电机驱动，编码器驱动，PID位置环，运动路径规划等
 */
void Measure_task(void *arg)
{
	uint8_t queue_recv_item;
	BaseType_t ret;

	printf("Start Measuring Task\r\n");

	adc_instance1_dma_init((uint32_t)&g_adc_dma_buf);                     	/* ADC DMA初始化 */
	adc_instance2_dma_init((uint32_t)&g_adc3_dma_buf);

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
				SG_LOG(SEVERITY_INFO, "Got START Measure Command");
				tsk_Move_Execute(&ret);
				break;

			case STOP_MEASURE:
            	SG_LOG(SEVERITY_INFO, "Got STOP Measure Command");
#if DEBUG_MODE
            	{
					uint32_t adc1x[ADC1_CH_NUM];
					uint32_t adc3x[ADC3_CH_NUM];

					/* 获取ADC采样值*/
					Get_ADC_Value(ADC_ADC1, ADC1_CH_NUM, adc1x);
					Get_ADC_Value(ADC_ADC3, ADC3_CH_NUM, adc3x);
            	}
#endif
				break;

			case DEBUG_MOVE:
				sliede_way_test(g_measureCfg.debug_config.distance,
								g_measureCfg.debug_config.direction);
				break;
		}
		vTaskDelay(1000);
	}
}


/**
 * USB事件处理轮询任务
 */
extern USBH_HandleTypeDef g_hUSBHost;    /* USB Host处理结构体 */
void USBProc_task(void *arg)
{
	/*	每10ms处理一次USB事件 */
	for(;;)
	{
		USBH_Process(&g_hUSBHost);
		vTaskDelay(10);
	}
}


void start_task(void *pvParameter)
{
	taskENTER_CRITICAL();

	BaseType_t ret_val = pdFALSE;

	tsk_init_queues();

	init_measureCfg(0.2, 0.2, 0.2);

	//创建任务
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
	//创建测量任务
	ret_val = xTaskCreate(Measure_task,
						  Measure_TASK_NAME,
						  Measure_Task_Stack,
						  NULL,
						  Measure_Task_Prio,
						  &Measure_Handle_t);
	if( pdTRUE != ret_val){
	printf("Create Task %s failed\r\n", Measure_TASK_NAME);
	}

//创建心跳任务
	ret_val  = xTaskCreate(HeartBeat_task,
						HEARTBEAT_TASK_NAME,
						HeartBeat_Task_Stack,
						NULL,
						HeartBeat_Task_Prio,
						&HeartBeat_Handle_t);
	if( pdTRUE != ret_val){
		printf("Create Task %s failed\r\n", HEARTBEAT_TASK_NAME);
	}

	//创建串口任务
	ret_val = xTaskCreate(UsartCommu_task,
						  USART_TASK_NAME,
						  Usart_Task_Stack,
						  NULL,
						  Usart_Task_Prio,
						  &Usart_Handle_t);
	if( pdTRUE != ret_val){
		printf("Create Task %s failed\r\n", USART_TASK_NAME);
	}

	//创建USB事件处理任务
	ret_val = xTaskCreate(USBProc_task,
						  USB_TASK_NAME,
						  USB_Proc_Task_Stack,
						  NULL,
						  USB_Proc_Task_Prio,
						  &Usart_Handle_t);
	if( pdTRUE != ret_val){
		printf("Create Task %s failed\r\n", USB_TASK_NAME);
	}

	printf("Create Tasks Success\r\n");
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}





