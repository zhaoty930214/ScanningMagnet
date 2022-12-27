#ifndef __TSK_FUNCTIONS_H
#define __TSK_FUNCTIONS_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "pid.h"
#include "stepper_motor.h"
#include "Measure_Types.h"
#include "exfuns.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "SGLog.h"
#include "adc.h"
#include "my_tasks.h"

/**
 * ����������
 */
/**************************************/
#define ENABLE_UART_DRAW	0		//Ϊ1ʱʹ�ܴ��ڴ�ӡ����·���켣
#define ACTUAL_MOVE_MOTOR	0		//Ϊ0ʱʹ��PID���ƻ���·����
#define DEBUG_MODE			0		//Ϊ0ʱ��·���ֵ����õĴ���

/**************************************/


extern MeasureConfig_t g_measureCfg;
extern uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];     /* ADC1 DMA BUF */
extern uint16_t g_adc3_dma_buf[ADC3_DMA_BUF_SIZE];   /* ADC3 DMA BUF */


#define MOTOR_START()    do{HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);	\
							HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH2); }while(0)





//�����ʱ�����
extern TimerHandle_t	AutoReloadTimer_Handle;

//�ṩ�����ڽ�������ʹ�õ�ȫ����Ϣ���о��
extern  xQueueHandle	Queue_Usart;
extern	xQueueHandle	Queue_Measure;
extern	xQueueHandle	Queue_MotorReady;
extern	xQueueHandle	Queue_Sensor_Data;
extern  xQueueHandle	Queue_ADC1_Sample_Complete;
extern  xQueueHandle	Queue_ADC3_Sample_Complete;
/*tsk��ͷ�ĺ�������ʾӦ����task���������е���*/
void tsk_init_queues(void);
void tsk_Move_Execute(void *param);
void tsk_Move_step(uint8_t axis_id, int set_point);
bool tsk_WorkFlow_Handler(void);
void tsk_USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);	/*USB �¼�ѭ��*/



void init_measureCfg(float x, float y, float z);
void draw_rectangle(bool dir_x, bool dir_y, float x_proportion, float y_proportion);
void update_stepper_params(void);

void sliede_way_test(uint32_t pulseCnt, bool dir_t);

/*����������*/
void initialize_WorkFlow_Api(void);
void Move_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow);
void ADC_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow);
void Record_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow);

/* �ļ��������*/
FRESULT Create_Measure_file(void);

/* ADC��������*/
void Get_ADC_Value(ADC_TypeDef * adcx, int channel_num, uint32_t *pbuff);

FRESULT appendMeasuringFile(char *contents);
FRESULT CloseMeasuringFile();

/*�궨����ߴ���λ*/
bool tsk_limit_check(int x, int y, int z);

void GetCoordinates(float *x, float *y, float *z);
#endif
