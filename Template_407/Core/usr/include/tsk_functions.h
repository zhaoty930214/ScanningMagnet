#ifndef __TSK_FUNCTIONS_H
#define __TSK_FUNCTIONS_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "pid.h"
#include "stepper_motor.h"
#include "Measure_Types.h"
/**
 * ����������
 */
/**************************************/
#define ENABLE_UART_DRAW	0
#define ACTUAL_MOVE_MOTOR	1
/**************************************/

#define MOTOR_START()    do{HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);	\
							HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH2); }while(0)





//�����ʱ�����
extern TimerHandle_t	AutoReloadTimer_Handle;

//�ṩ�����ڽ�������ʹ�õ�ȫ����Ϣ���о��
extern  xQueueHandle	Queue_Usart;
extern	xQueueHandle	Queue_Measure;
extern	xQueueHandle	Queue_MotorReady;


/*tsk��ͷ�ĺ�������ʾӦ����task���������е���*/
void tsk_init_queues(void);
void tsk_Move_Execute(void *param);
void tsk_Move_step(uint8_t axis_id, int set_point);


void init_measureCfg(float x, float y, float z);
void draw_rectangle(bool dir_x, bool dir_y, float x_proportion, float y_proportion);
void update_stepper_params(void);
bool tsk_Measuring(void);
void init_input_io(void);
void sliede_way_test(uint32_t pulseCnt, bool dir_t);

/*����������*/
void initialize_WorkFlow_Api(void);
void Move_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow);
void ADC_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow);
void Record_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow);
#endif
