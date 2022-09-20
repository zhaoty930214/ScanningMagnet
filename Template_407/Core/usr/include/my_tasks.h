/*
 * my_tasks.h
 *
 *  Created on: Aug 23, 2022
 *      Author: tianyu.zhao
 */

#ifndef INC_MY_TASKS_H_
#define INC_MY_TASKS_H_
#include "FreeRTOS.h"
#include "task.h"		//ʹ������֧��
#include "queue.h"		//ʹ����Ϣ����֧��
#include "Measure_Types.h"






#define STATISTIC_TASK_NAME		"tStatistic"
#define Statistic_Task_Stack	256
#define Statistic_Task_Prio		1


#define START_TASK_NAME		"tStart"
#define START_Task_Stack	128
#define START_Task_Prio		1


#define HEARTBEAT_TASK_NAME		"tHeartBeat"
#define HeartBeat_Task_Stack	256
#define HeartBeat_Task_Prio		4

#define USART_TASK_NAME		"tUsart"
#define Usart_Task_Stack	512
#define Usart_Task_Prio		6

#define Measure_TASK_NAME		"tMeasure"
#define Measure_Task_Stack	512
#define Measure_Task_Prio	5

extern MeasureConfig_t g_measureCfg;
extern xQueueHandle	Queue_MotorReady;

void start_task(void *pvParameter);

/**
 * �˴�����һЩ�����������õĺ���
 */
//void Move_Execute(void *param);

#endif /* INC_MY_TASKS_H_ */
