#include "tsk_functions.h"
#include "Measure_Types.h"
#include "Measure_Constants.h"
#include "LED.h"
#include <stdbool.h>
#include "stepper_tim.h"
#include "gtim.h"

extern MeasureConfig_t g_measureCfg;

/*
 * ����������ṹ��
 */
static WorkFlow_t sg_WorkFlow;
//{
//	{
//	 .MainFlow = Main_WorkFlow_Move,
//	 .SubFlow  = Sub_WorkFlow_X
//	},
//	{
//	 .mainWorkFlow_handler = api_axis_x,
//	 .subWorkFlow_handler = NULL
//	}
//};


/**
 * �����ʱ�����
 */
TimerHandle_t	AutoReloadTimer_Handle;

/**
 * ��Ϣ���о��
 */
xQueueHandle	Queue_Usart;
xQueueHandle	Queue_Measure;
xQueueHandle	Queue_MotorReady;

/*
 * �����ʱ���ص�
 */
void AutoReloadCallback(TimerHandle_t xTimer)
{
	static int cnt = 0;
	cnt++;
	if(cnt == 10)
	{
		cnt = 0;
		xTimerStop(AutoReloadTimer_Handle, 0);
		LED1(0);
	}
	LED1_TOGGLE();
}


void tsk_init_queues(void)
{
	//����������ڶ�ʱ��, ���ڶ�ʱ��������100ms(100 ��ʱ�ӽ���),����ģʽ
	AutoReloadTimer_Handle=xTimerCreate((const char*            )"AutoReloadTimer",
										(TickType_t             )100,
										(UBaseType_t            )pdTRUE,
										(void*)1,
										(TimerCallbackFunction_t)AutoReloadCallback);

	//������Ϣ����
	Queue_Usart = xQueueCreate(Usart_QUEUE_LENGTH, QUEUE_SIZE);
	if(Queue_Usart == NULL)
	{
		printf("Create Queue_Usart failed\r\n");
	}

	Queue_Measure = xQueueCreate(Measure_QUEUE_LENGTH, QUEUE_SIZE);

	if(Queue_Measure == NULL)
	{
		printf("Create Queue_Measure failed\r\n");
	}

	/*�����λ��ֻ����д��1�����ȣ�X&&Y&&Z����λʱ���ܴ�PID�ж�������Ϣ����д��*/
	Queue_MotorReady = xQueueCreate(MotorReady_QUEUE_LENGTH, QUEUE_SIZE);
	if(Queue_MotorReady == NULL)
	{
		printf("Create Queue_MotorReady\r\n");
	}
}

/*��ʼ���û����ڵ�Ŀ�꣨��Ӧ�ĸ�����Ŀ�꣩*/
float *MOTOR_X_GO = (float*)(&g_location_pid.SetPoint);
float *MOTOR_Y_GO = (float*)(&g_location_pid.SetPoint);
float *MOTOR_Z_GO = (float*)(&g_location_pid.SetPoint);

bool tsk_WaitMoveComplete(uint8_t axis_id)
{
	bool ret;
	int queue_recv_item;
	//�����ȴ�,����˶����
	ret = xQueueReceive(Queue_MotorReady, &queue_recv_item, portMAX_DELAY);
	if(pdPASS != ret)
	{
		printf("Waiting for motor ready queue failed\r\n");
	}
	else
	{
		printf("Waiting motor ready success %d\r\n", queue_recv_item);
	}

	switch(axis_id)
	{
	case Axis_x:
	case Axis_y:
	case Axis_z:
		if(queue_recv_item == axis_id)
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
		break;
	}

	return ret;
}






/**
 * �򵥵Ĵ�����������κ���
 */
void draw_rectangle(bool dir_x, bool dir_y, float x_proportion, float y_proportion)
{

	int i;
	int j; //(dir_y == CW)? 0:1;
	int y_limit = y_proportion * Y_Items;
	int x_limit = x_proportion * X_Items;


	//���������˶�ʱ��y_limitҲ�Ǵ��¿�ʼ����ֻ�����0~9��
	//��cmp_obj��ֵ��Ϊ10-0~9 = 1~10,
	//��ʱj������Χ0~9��Ȼ������ ��������j��1��ʼ
	int cmp_obj = (dir_y == CW)?y_limit:Y_Items-y_limit-1;

//	char s_finish[10]="**********";
//	char s_unfinish[10]="----------";
	switch(dir_y)
	{
		case CW:					//Y���������뻭ͼ����һ��
		case CCW:
		{
			//���б���
			for(j=0; j<Y_Items; j++)
			{
//				if(j == 0)
//					printf("Current X dir is %c\r\n", (g_measureCfg.dir_x==CW)?'+':'-');
				//���������˶�ʱ���Ƚ�Ŀ��Ϊ���������;��֮Ϊδ�������
				if(j<cmp_obj)
				{
					//����һ��ʱ����Ҫ����
					if(dir_y == CW)
						printf("**********\r\n");
					else
						printf("----------\r\n");
					continue;
				}
				else if(j> cmp_obj)
				{
					//����һ��ʱ����Ҫ����
					if(dir_y == CW)
						printf("----------\r\n");
					else
						printf("**********\r\n");
					continue;
				}
				//�ҵ���ǰ�����˶���һ�У�������
				else if(j==cmp_obj)
				{
					for(i=0; i<(X_Items); i++)
					{
						//���������˶�ʱ���Ƚ�Ŀ��Ϊ���������;��֮Ϊδ�������
						if(dir_x == CW)
						{
							if(i<x_limit)
								printf("*");
							else
								printf("-");
						}
						else
						{
							if(i<(X_Items-x_limit-1))
								printf("-");
							else
								printf("*");
						}
					}
					//����һ��ʱ����Ҫ����
					printf("\r\n");
				}
			}
		}
		break;
	}
}


/**
 * ��ʼ��XYZ�����Ĭ�ϵ��ξ��룬��λmm
 * ������� @x, @y, @z �����Ĭ�ϵ����ƶ����룬��Ч��Χ0~10mm��С�����2λ��Ч
 */
void init_measureCfg(float x, float y, float z)
{
	assert(0 < x && x < 10.01);
	assert(0 < y && y < 10.01);
	assert(0 < z && z < 10.01);

	g_measureCfg.step_x = x;
	g_measureCfg.step_y = y;
	g_measureCfg.step_z = z;
}



void api_axis_x(void *param)
{

}



void ADC_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow)
{
	/*ADC����*/
	pWorkFlow->MainFlow = Main_WorkFlow_Record;
}

void Record_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow)
{
	/*���ݼ�¼*/
	pWorkFlow->MainFlow = Main_WorkFlow_Move;
}


/**
 *
 */
void update_stepper_params(void)
{
	/*����XYZ����ֵ*/
	g_measureCfg.x_index = 0;
	g_measureCfg.y_index = 0;
	g_measureCfg.z_index = 0;

	/*����������ԵĲ����ƴ���*/
	g_measureCfg.step_cnt_x = X_LENGTH/g_measureCfg.step_x;
	g_measureCfg.step_cnt_y = Y_LENGTH/g_measureCfg.step_y;
	g_measureCfg.step_cnt_z = Z_LENGTH/g_measureCfg.step_z;

	/*����������ԵĲ��������� = ÿ���׶�Ӧ��������*����������׵�λ. */
	g_measureCfg.pulse_cnt_x = PPM * g_measureCfg.step_x;
	g_measureCfg.pulse_cnt_y = PPM * g_measureCfg.step_y;
	g_measureCfg.pulse_cnt_z = PPM * g_measureCfg.step_z;

	//*User_SetPoint += pulse_cnt_x;
	printf("PPM is %f and | CntPerCycle_x is %d | Pulses_x is %d\r\n", PPM, g_measureCfg.step_cnt_x, g_measureCfg.pulse_cnt_x);
	printf("PPM is %f and | CntPerCycle_y is %d | Pulses_y is %d\r\n", PPM, g_measureCfg.step_cnt_y, g_measureCfg.pulse_cnt_y);
	printf("PPM is %f and | CntPerCycle_z is %d | Pulses_z is %d\r\n", PPM, g_measureCfg.step_cnt_z, g_measureCfg.pulse_cnt_z);

	printf("Estimate measuring time is %d seconds \r\n", g_measureCfg.step_cnt_x*
														 g_measureCfg.step_cnt_y*
														 g_measureCfg.step_cnt_z/100);
}





void tsk_Move_Execute(void *param)
{
	float p1,p2;
	uint32_t notify_value;


	update_stepper_params();

	/*��ʼ����ǰ���Ȼع�ԭ��P0�� α����*/
	//go_to_origin();

	/*�ع�ԭ��P0��Ѱ�Ҳ���ê�㣬�������׾������ԭ��P1*/
	//go_to_relative_origin();

	/*������Ҫ�����Ƿ������Աȷ��P1����λ*/
	//xQueueReceive(Queue_rltOrigin_confirm, &bd, portDelayMax);
	//

	/*һ����λ��һ���򴮿ڿ���̨����·��ͼ��,������λ�����������ţ�ƣ����밲���ϡ�
	*�����׾�����һ��Բ����������λ�ô�����ͼ. ���ǵ����ܼ��ź�ʡ��Ŀ�����
	*���õ����ÿ��XYƽ��������ʱ���»ص���ǰƽ������ԭ�㣬���ǲ���������λ��
	*��ˣ�Z�᷽��̶����䣬X��Y��ÿ���ߵ����Եľ�ͷ����Ҫ����			*/
	/*  .
	 * ...
	 *.....
	 * ...
	 *  .   */

	/*��������*/
	//make_moving_plan;
	//measuring module
	//step
	//get hall data

	/*��ʼ����ǰ�����ֵ֪ͨ*/
	notify_value = 0;
	xTaskNotify(xTaskGetCurrentTaskHandle(), notify_value, eSetValueWithOverwrite);

	while( tsk_Measuring() != true)
	{
		/*�������ֹͣ����*/
		notify_value = ulTaskNotifyTake(pdTRUE, 0);
		if(notify_value == NOTIFY_STOP_MEASURE)
		{
			printf("Test stop due to stop command \r\n");
			return ;
		}

		vTaskDelay(100);
		//������ȣ����ڴ�ӡ���ȱ����ָ�
//		#if ENABLE_UART_DRAW
//			p1 = (float) (g_measureCfg.x_index)/g_measureCfg.step_cnt_x;
//			p2 = (float) (g_measureCfg.y_index)/g_measureCfg.step_cnt_y;
//
//			if( (g_measureCfg.x_index < g_measureCfg.step_cnt_x) &&
//				(g_measureCfg.y_index < g_measureCfg.step_cnt_y) )
//			{
//				/*ѭ�����ⲿ��ͼ*/
//				draw_rectangle(g_measureCfg.dir_x, g_measureCfg.dir_y, p1, p2);
//				vTaskDelay(5);
//				for(int i=0; i<10; i++)
//				{
//					printf("\033[1A"); //�Ȼص���һ��
//				}
//			}
//		#endif
	}
	printf("Finish measure tracing task\r\n");
	//��֪�������룬���㲽��������
}



void tsk_Move_step(uint8_t axis_id, int set_point)
{
#if	1
	switch(axis_id)
	{
	case Axis_x:
		g_encoder_t.g_run_flag = 1;
		if(g_measureCfg.dir_x)
			*MOTOR_X_GO += set_point;
		else
			*MOTOR_X_GO -= set_point;
		break;
	case Axis_y:
		//*MOTOR_Y_GO += set_point;
		break;
	case Axis_z:
		//*MOTOR_Z_GO += set_point;
		break;
	}

	tsk_WaitMoveComplete(axis_id);
#endif
}


/**
 *�����ƶ�����
 *@����, param: �������ṹ��ָ�����
 */
void Move_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow)
{
	MainWorkFlow_List_t *pmain_flow = &pWorkFlow->MainFlow;
	SubWorkFlow_List_t  *psub_flow  = &pWorkFlow->SubFlow;

	switch(*psub_flow)
	{
	/*��Ϊ�����ⲿ��ͼ�����Ե�y_index = step_cnt_y��Y�ķ�ת�ж�ʱ�����ı�X�ķ��� */
	case Sub_WorkFlow_X:
		/*��ǰX���г�δ���*/
		if(g_measureCfg.x_index < g_measureCfg.step_cnt_x)
		{
			g_measureCfg.x_index++;
			//X��ջ��ƶ�
			tsk_Move_step(Axis_x, g_measureCfg.pulse_cnt_x);

			*pmain_flow = Main_WorkFlow_ADC;
		}
		else
		{
//			if(g_measureCfg.y_index != g_measureCfg.step_cnt_y)
//			{
//				g_measureCfg.dir_x   = !g_measureCfg.dir_x;
//				printf("Axis X direction toggle\r\n");
//			}
			g_measureCfg.dir_x   = !g_measureCfg.dir_x;
			printf("Axis X direction toggle\r\n");
			g_measureCfg.x_index = 0;
			*psub_flow = Sub_WorkFlow_Y;
		}
		break;

	case Sub_WorkFlow_Y:
		/*��ǰY���г�δ���*/
		if(g_measureCfg.y_index < g_measureCfg.step_cnt_y)
		{
			g_measureCfg.y_index++;
			//X��ջ��ƶ�
			tsk_Move_step(Axis_y, g_measureCfg.pulse_cnt_y);

			*psub_flow = Sub_WorkFlow_X;
		}
		else
		{
			g_measureCfg.dir_y   = !g_measureCfg.dir_y;
			g_measureCfg.y_index = 0;
			*psub_flow = Sub_WorkFlow_Z;
		}
		break;

	case Sub_WorkFlow_Z:
		/*��ǰY���г�δ���*/
		if(g_measureCfg.z_index < g_measureCfg.step_cnt_z)
		{
			g_measureCfg.z_index++;
			//X��ջ��ƶ�
			tsk_Move_step(Axis_z, g_measureCfg.pulse_cnt_z);
			*psub_flow = Sub_WorkFlow_X;
		}
		else
		{
			g_measureCfg.z_index = 0;
			*psub_flow = Sub_WorkFlow_Finish;
		}
		break;

	default:
		break;
	}
}



/**
 * ��������ʼ
 * 1.��ȡ��ǰ������״̬.
 * 2.������״̬��Main_WorkFlow_Move-> Main_WorkFlow_ADC ->Main_WorkFlow_Move...
 * 3.�ӹ���״̬��Main_WorkFlow_Move-> Sub_WorkFlow_X
 * 							   -> Sub_WorkFlow_Y
 * 							   -> Sub_WorkFlow_Z
 * 4.��״̬ת����Move -> ADC
 * 5.��״̬ת��
 *   X,Y,Z�Ƿ��˶����������Ե��
 *		X���Ե -> ��״̬��Ϊ	 Y��
 *		Y���Ե -> ��״̬��Ϊ Z��
 *
 */
bool tsk_Measuring(void)
{
	static WorkFlow_Level_t s_workFlowLevel = {
			.MainFlow = Main_WorkFlow_Move,
			.SubFlow  = Sub_WorkFlow_X
	};

	switch(s_workFlowLevel.MainFlow){
	case Main_WorkFlow_Move:
		Move_WorkFlow_handler(&s_workFlowLevel);
		break;

	case Main_WorkFlow_ADC:
		ADC_WorkFlow_handler(&s_workFlowLevel);
		break;

	case Main_WorkFlow_Record:
		Record_WorkFlow_handler(&s_workFlowLevel);
		break;
	}

	if(Sub_WorkFlow_Finish == s_workFlowLevel.SubFlow)
	{
		s_workFlowLevel.SubFlow  = Sub_WorkFlow_X;
		s_workFlowLevel.MainFlow = Main_WorkFlow_Move;
		return true;
	}
	else
		return false;
}


void init_input_io(void)
{
    GPIO_InitTypeDef gpio_init_struct;                 /* GPIO���ò����洢���� */
    __HAL_RCC_GPIOG_CLK_ENABLE();    				   /* PG��ʱ��ʹ�� */

    gpio_init_struct.Pin = GPIO_PIN_15;                 /* GP8���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;           /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;     			   /* ���� */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);           /* PG8����ģʽ����,���� */
}


//������������ƶ�1mm����Ӧ��������
#define PULSE_PER_MM	50
extern  volatile uint16_t g_step_angle;                         /* ���õĲ����Ƕ� */

#define SETPOINT_PULSE_PER_MM	ENCODER_SPR/MPR

int g_pulse_count = 0;
void sliede_way_test(uint32_t dis_mm, bool dir_t)
{
	/*�ƶ�����= pulse per milimeter*milimeter.*/

	g_step_angle = SETPOINT_PULSE_PER_MM*dis_mm;
	printf("Ready to move X %dmm @ direction %d\r\n", dis_mm, dir_t);
	printf("Related pulse count is %d\r\n", g_step_angle);

	//ST1_DIR(dir_t);
	if(dir_t)
		*MOTOR_X_GO += SETPOINT_PULSE_PER_MM*dis_mm;
	else
		*MOTOR_X_GO -= SETPOINT_PULSE_PER_MM*dis_mm;

	g_encoder_t.g_run_flag = 1;
	printf("Direction is %d\r\n", dir_t);
	//stepper_star(STEPPER_MOTOR_1);

	//g_step_angle = 2;
	//g_pulse_count = 800;
	//stepper_star(STEPPER_MOTOR_1);

//	for(int i=0; i<100;i++)
//	{
//		ST1_DIR(dir);
//		dir = !dir;
//
//		stepper_star(STEPPER_MOTOR_1);
//		while(g_step_angle > 0);	/*��������ǰ��������*/
//
//		g_step_angle = PULSE_PER_MM*dis_mm;
//		vTaskDelay(3000);
//	}

}

WorkFlow_Handler_t handler_api = {
	.mainWorkFlow_api_adc = Move_WorkFlow_handler,
};


void initialize_WorkFlow_Api(void)
{
	sg_WorkFlow.workflow_level.MainFlow = Main_WorkFlow_Move;
	sg_WorkFlow.workflow_level.SubFlow  = Sub_WorkFlow_X;

	sg_WorkFlow.workflow_handler.mainWorkFlow_api_move   = &Move_WorkFlow_handler;
	sg_WorkFlow.workflow_handler.mainWorkFlow_api_adc    = &ADC_WorkFlow_handler;
	sg_WorkFlow.workflow_handler.mainWorkFlow_api_record = &Record_WorkFlow_handler;
	sg_WorkFlow.workflow_handler.subWorkFlow_api_moveX = NULL;
	//sg_WorkFlow.workflow_handler.
}

