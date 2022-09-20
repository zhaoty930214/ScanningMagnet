#ifndef __MEASURE_TYPES_H
#define __MEASURE_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct MeasureConfig
{
	uint8_t cmd_channel;		/*记录不同的命令编号*/
	float step_x;				/*x轴步进距离,单位mm*/
	float step_y;				/*y轴步进距离,单位mm*/
	float step_z;				/*z轴步进距离,单位mm*/
	float amplifier_current;	/*扫描磁铁放大器电流值*/
	uint16_t step_cnt_x;		/*x轴走完一行，需要测量的点数*/
	uint16_t step_cnt_y;		/*y轴走完一行，需要测量的点数*/
	uint16_t step_cnt_z;		/*z轴走完一行，需要测量的点数*/
	uint16_t pulse_cnt_x;		/*x轴每走一次，期望的脉冲输出值(编码器脉冲)*/
	uint16_t pulse_cnt_y;		/*y轴每走一次，期望的脉冲输出值(编码器脉冲)*/
	uint16_t pulse_cnt_z;		/*z轴每走一次，期望的脉冲输出值(编码器脉冲)*/
	int	x_index;				/*当前x轴行程系数*/
	int y_index;				/*当前y轴行程系数*/
	int z_index;				/*当前z轴行程系数*/
	bool dir_x;
	bool dir_y;
} __attribute__((packed)) MeasureConfig_t;

typedef enum MainWorkFlow_List
{
	Main_WorkFlow_Move,
	Main_WorkFlow_ADC,
	Main_WorkFlow_Record,
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
	void (*mainWorkFlow_api_move)(void *param);
	void (*mainWorkFlow_api_adc)(void *param);
	void (*mainWorkFlow_api_record)(void *param);

	void (*subWorkFlow_api_moveX)(void *param);
	void (*subWorkFlow_api_moveY)(void *param);
	void (*subWorkFlow_api_moveZ)(void *param);
} __attribute__((packed)) WorkFlow_Handler_t;;

typedef struct WorkFlow
{
	WorkFlow_Level_t	workflow_level;
	WorkFlow_Handler_t	workflow_handler;
} __attribute__((packed)) WorkFlow_t;


/*Axis ID*/
enum Axis_ID{
	Axis_x,
	Axis_y,
	Axis_z
};

/**
 * 中断处理函数结构体
 */



#endif

