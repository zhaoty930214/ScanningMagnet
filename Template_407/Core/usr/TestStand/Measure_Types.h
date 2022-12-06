#ifndef __MEASURE_TYPES_H
#define __MEASURE_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Measure_Constants.h"
/*********************串口协议包内容*****************************/

//模板结构体
typedef struct
{
	uint8_t channel_id;			//命令号的所属ID
	void*	pstorage;		//存储数据的结构体
	uint8_t	pack_size;			//期望的数据包大小
	void (*data_parse_handler)(void *arg, uint8_t index);
} Uart_Handler_t;

typedef struct MeasuringConfig
{
	volatile float step_x;				/*x轴步进距离,单位mm*/
	volatile float step_y;				/*y轴步进距离,单位mm*/
	volatile float step_z;				/*z轴步进距离,单位mm*/
	volatile float amplifier_current;	/*扫描磁铁放大器电流值*/
} __attribute__((packed)) MeasuringConfig_t;

typedef struct DebugConfig
{
	uint8_t axis_id;			//单轴调试模式，要移动的轴ID
	uint8_t	direction;			//移动的方向
	float	distance;			//移动的距离，单位mm
} __attribute__((packed)) DebugConfig_t;


typedef struct TimeConfig
{
	uint16_t year;
	uint8_t	 month;
	uint8_t	 day;
	uint8_t  hour;
	uint8_t	 minute;
	uint8_t  second;
} __attribute__((packed)) TimeConfig_t;

/*********************串口协议包内容*****************************/

/******************测量过程中的参数，由计算得出**********************/

/*Axis ID*/
enum Axis_ID{
	Axis_x = 0,
	Axis_y,
	Axis_z,
	Axis_Count
};

typedef struct MeasureParams
{
	uint16_t step_cnt_x[Axis_Count];		/*x轴走完一行，需要测量的点数*/
	//uint16_t step_cnt_y;		/*y轴走完一行，需要测量的点数*/
	//uint16_t step_cnt_z;		/*z轴走完一行，需要测量的点数*/
	uint16_t pulse_cnt_x[Axis_Count];		/*x轴每走一次，期望的脉冲输出值(编码器脉冲)*/
	//uint16_t pulse_cnt_y;		/*y轴每走一次，期望的脉冲输出值(编码器脉冲)*/
	//uint16_t pulse_cnt_z;		/*z轴每走一次，期望的脉冲输出值(编码器脉冲)*/
	int	x_index[Axis_Count];				/*当前x轴行程系数*/
	//int y_index;				/*当前y轴行程系数*/
	//int z_index;				/*当前z轴行程系数*/
	bool dir_x[Axis_Count];
	//bool dir_y;
} __attribute__((packed)) MeasureParams_t;
/******************测量过程中的参数，由计算得出**********************/

typedef struct MeasureConfig
{
	uint8_t cmd_channel;		/*记录不同的命令编号*/
	MeasuringConfig_t 		measure_config;
	DebugConfig_t			debug_config;
	MeasureParams_t			measure_params;
	TimeConfig_t			time_config;
	volatile uint8_t recv_data[DEBUG_REV_MAX_LEN];
} __attribute__((packed)) MeasureConfig_t;


typedef enum MainWorkFlow_List
{
	Main_WorkFlow_Move = 0,
	Main_WorkFlow_ADC,
	Main_WorkFlow_Record,
	Main_WorkFlow_count,
}MainWorkFlow_List_t;

typedef enum SubWorkFlow_List
{
	Sub_WorkFlow_X =0,
	Sub_WorkFlow_Y,
	Sub_WorkFlow_Z,
	Sub_WorkFlow_Finish,
	Sub_WorkFlow_Count,
}SubWorkFlow_List_t;

typedef struct WorkFlow_Level
{
	MainWorkFlow_List_t MainFlow;
	SubWorkFlow_List_t SubFlow;
} __attribute__((packed)) WorkFlow_Level_t;

typedef struct WorkFlow_Handler
{
	void (*mainWorkFlow_api[Main_WorkFlow_count])(WorkFlow_Level_t *param);
//	void (*mainWorkFlow_api_move)(void *param);
//	void (*mainWorkFlow_api_adc)(void *param);
//	void (*mainWorkFlow_api_record)(void *param);

	void (*subWorkFlow_api_moveX)(void *param);
	void (*subWorkFlow_api_moveY)(void *param);
	void (*subWorkFlow_api_moveZ)(void *param);
} __attribute__((packed)) WorkFlow_Handler_t;;

typedef struct WorkFlow
{
	WorkFlow_Level_t	workflow_level;
	WorkFlow_Handler_t	workflow_handler;
} __attribute__((packed)) WorkFlow_t;



/*串口注册结构体*/
typedef struct Uart_Struct
{
	uint8_t channel_id;
	uint8_t pack_length;
	void * pstorage;
	void (*data_handler)(void *arg, uint8_t index);
}Uart_Struct_t;






typedef enum EventLogSeverity{
    /** 指示未提供严重性级别或无效的严重性级别 */
    SEVERITY_INVALID  = 0,

    /** 指示记录后将停止系统的最高严重性消息 */
    SEVERITY_FATAL    = 1,

	/** 表示包含对系统运行至关重要的信息的消息 */
	SEVERITY_CRITICAL = 2,

	/** 指示本条消息表示关于系统的警告 */
    SEVERITY_WARNING  = 3,

    /** 指示本条消息表示一个信息提醒的目的 */
    SEVERITY_INFO     = 4,

    /** 指示本条消息只会出现在调试窗口，而不会记录到LOG文件中 */
    SEVERITY_DEBUG    = 5
} EventLogSeverity_t;
/**
 * 中断处理函数结构体
 */



#endif

