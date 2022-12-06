#ifndef __MEASURE_TYPES_H
#define __MEASURE_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Measure_Constants.h"
/*********************����Э�������*****************************/

//ģ��ṹ��
typedef struct
{
	uint8_t channel_id;			//����ŵ�����ID
	void*	pstorage;		//�洢���ݵĽṹ��
	uint8_t	pack_size;			//���������ݰ���С
	void (*data_parse_handler)(void *arg, uint8_t index);
} Uart_Handler_t;

typedef struct MeasuringConfig
{
	volatile float step_x;				/*x�Ჽ������,��λmm*/
	volatile float step_y;				/*y�Ჽ������,��λmm*/
	volatile float step_z;				/*z�Ჽ������,��λmm*/
	volatile float amplifier_current;	/*ɨ������Ŵ�������ֵ*/
} __attribute__((packed)) MeasuringConfig_t;

typedef struct DebugConfig
{
	uint8_t axis_id;			//�������ģʽ��Ҫ�ƶ�����ID
	uint8_t	direction;			//�ƶ��ķ���
	float	distance;			//�ƶ��ľ��룬��λmm
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

/*********************����Э�������*****************************/

/******************���������еĲ������ɼ���ó�**********************/

/*Axis ID*/
enum Axis_ID{
	Axis_x = 0,
	Axis_y,
	Axis_z,
	Axis_Count
};

typedef struct MeasureParams
{
	uint16_t step_cnt_x[Axis_Count];		/*x������һ�У���Ҫ�����ĵ���*/
	//uint16_t step_cnt_y;		/*y������һ�У���Ҫ�����ĵ���*/
	//uint16_t step_cnt_z;		/*z������һ�У���Ҫ�����ĵ���*/
	uint16_t pulse_cnt_x[Axis_Count];		/*x��ÿ��һ�Σ��������������ֵ(����������)*/
	//uint16_t pulse_cnt_y;		/*y��ÿ��һ�Σ��������������ֵ(����������)*/
	//uint16_t pulse_cnt_z;		/*z��ÿ��һ�Σ��������������ֵ(����������)*/
	int	x_index[Axis_Count];				/*��ǰx���г�ϵ��*/
	//int y_index;				/*��ǰy���г�ϵ��*/
	//int z_index;				/*��ǰz���г�ϵ��*/
	bool dir_x[Axis_Count];
	//bool dir_y;
} __attribute__((packed)) MeasureParams_t;
/******************���������еĲ������ɼ���ó�**********************/

typedef struct MeasureConfig
{
	uint8_t cmd_channel;		/*��¼��ͬ��������*/
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



/*����ע��ṹ��*/
typedef struct Uart_Struct
{
	uint8_t channel_id;
	uint8_t pack_length;
	void * pstorage;
	void (*data_handler)(void *arg, uint8_t index);
}Uart_Struct_t;






typedef enum EventLogSeverity{
    /** ָʾδ�ṩ�����Լ������Ч�������Լ��� */
    SEVERITY_INVALID  = 0,

    /** ָʾ��¼��ֹͣϵͳ�������������Ϣ */
    SEVERITY_FATAL    = 1,

	/** ��ʾ������ϵͳ����������Ҫ����Ϣ����Ϣ */
	SEVERITY_CRITICAL = 2,

	/** ָʾ������Ϣ��ʾ����ϵͳ�ľ��� */
    SEVERITY_WARNING  = 3,

    /** ָʾ������Ϣ��ʾһ����Ϣ���ѵ�Ŀ�� */
    SEVERITY_INFO     = 4,

    /** ָʾ������Ϣֻ������ڵ��Դ��ڣ��������¼��LOG�ļ��� */
    SEVERITY_DEBUG    = 5
} EventLogSeverity_t;
/**
 * �жϴ������ṹ��
 */



#endif

