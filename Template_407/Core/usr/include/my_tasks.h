/*
 * my_tasks.h
 *
 *  Created on: Aug 23, 2022
 *      Author: tianyu.zhao
 */

#ifndef INC_MY_TASKS_H_
#define INC_MY_TASKS_H_

#include "FreeRTOS.h"
#include "task.h"		//使用任务支持
#include "queue.h"		//使用消息队列支持
#include "SGLog.h"
#include "Measure_Types.h"
#include "adc.h"




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

#define Measure_TASK_NAME	"tMeasure"
#define Measure_Task_Stack	512
#define Measure_Task_Prio	5


#define USB_TASK_NAME		"tUSBProc"
#define USB_Proc_Task_Stack	512
#define USB_Proc_Task_Prio	5

extern MeasureConfig_t g_measureCfg;

void start_task(void *pvParameter);

/**
 * 此处定义一些由任务函数调用的函数
 */
//void Move_Execute(void *param);

#endif /* INC_MY_TASKS_H_ */
