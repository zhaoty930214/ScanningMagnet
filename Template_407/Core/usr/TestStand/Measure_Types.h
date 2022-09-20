#ifndef __MEASURE_TYPES_H
#define __MEASURE_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct MeasureConfig
{
	uint8_t cmd_channel;		/*��¼��ͬ��������*/
	float step_x;				/*x�Ჽ������,��λmm*/
	float step_y;				/*y�Ჽ������,��λmm*/
	float step_z;				/*z�Ჽ������,��λmm*/
	float amplifier_current;	/*ɨ������Ŵ�������ֵ*/
	uint16_t step_cnt_x;		/*x������һ�У���Ҫ�����ĵ���*/
	uint16_t step_cnt_y;		/*y������һ�У���Ҫ�����ĵ���*/
	uint16_t step_cnt_z;		/*z������һ�У���Ҫ�����ĵ���*/
	uint16_t pulse_cnt_x;		/*x��ÿ��һ�Σ��������������ֵ(����������)*/
	uint16_t pulse_cnt_y;		/*y��ÿ��һ�Σ��������������ֵ(����������)*/
	uint16_t pulse_cnt_z;		/*z��ÿ��һ�Σ��������������ֵ(����������)*/
	int	x_index;				/*��ǰx���г�ϵ��*/
	int y_index;				/*��ǰy���г�ϵ��*/
	int z_index;				/*��ǰz���г�ϵ��*/
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
 * �жϴ������ṹ��
 */



#endif

