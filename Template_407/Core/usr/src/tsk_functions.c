#include "tsk_functions.h"
#include "Measure_Constants.h"
#include "LED.h"
#include "stepper_tim.h"
#include "gtim.h"
#include "adc.h"
#include "rtc.h"

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

	g_measureCfg.measure_config.step_x = x;
	g_measureCfg.measure_config.step_y = y;
	g_measureCfg.measure_config.step_z = z;
}





void api_axis_x(void *param)
{

}





/**
 *
 */
void update_stepper_params(void)
{
	/*����XYZ����ֵ*/
	g_measureCfg.measure_params.x_index[Axis_x] = 0;
	g_measureCfg.measure_params.x_index[Axis_y] = 0;
	g_measureCfg.measure_params.x_index[Axis_z] = 0;

	g_measureCfg.measure_params.dir_x[Axis_x] = AXIS_X_DEFAULT_DIRECTION;
	g_measureCfg.measure_params.dir_x[Axis_y] = AXIS_Y_DEFAULT_DIRECTION;

	/*����������ԵĲ����ƴ���*/
	g_measureCfg.measure_params.step_cnt_x[Axis_x] = X_LENGTH/g_measureCfg.measure_config.step_x;
	g_measureCfg.measure_params.step_cnt_x[Axis_y] = Y_LENGTH/g_measureCfg.measure_config.step_y;
	g_measureCfg.measure_params.step_cnt_x[Axis_z] = Z_LENGTH/g_measureCfg.measure_config.step_z;

	/*����������ԵĲ��������� = ÿ���׶�Ӧ��������*����������׵�λ. */
	g_measureCfg.measure_params.pulse_cnt_x[Axis_x] = PPM * g_measureCfg.measure_config.step_x;
	g_measureCfg.measure_params.pulse_cnt_x[Axis_y] = PPM * g_measureCfg.measure_config.step_y;
	g_measureCfg.measure_params.pulse_cnt_x[Axis_z] = PPM * g_measureCfg.measure_config.step_z;

	//*User_SetPoint += pulse_cnt_x;
	printf("PPM is %f and | CntPerCycle_x is %d | Pulses_x is %d\r\n", PPM, g_measureCfg.measure_params.step_cnt_x[Axis_x], g_measureCfg.measure_params.pulse_cnt_x[Axis_x]);
	printf("PPM is %f and | CntPerCycle_y is %d | Pulses_y is %d\r\n", PPM, g_measureCfg.measure_params.step_cnt_x[Axis_y], g_measureCfg.measure_params.pulse_cnt_x[Axis_y]);
	printf("PPM is %f and | CntPerCycle_z is %d | Pulses_z is %d\r\n", PPM, g_measureCfg.measure_params.step_cnt_x[Axis_z], g_measureCfg.measure_params.pulse_cnt_x[Axis_z]);

	printf("Estimate measuring time is %d seconds \r\n", g_measureCfg.measure_params.step_cnt_x[Axis_x]*
														 g_measureCfg.measure_params.step_cnt_x[Axis_y]*
														 g_measureCfg.measure_params.step_cnt_x[Axis_z]/100);
	uint8_t buf_size = 80;
	char *buf = (char *) malloc(buf_size);

	if(buf)
	{
		snprintf(buf, buf_size, "PPM is %f and | CntPerCycle_x is %d | Pulses_x is %d\r\n",
					         PPM, g_measureCfg.measure_params.step_cnt_x[Axis_x], g_measureCfg.measure_params.pulse_cnt_x[Axis_x]);
		SG_LOG(SEVERITY_INFO, buf);

		snprintf(buf, buf_size, "PPM is %f and | CntPerCycle_y is %d | Pulses_y is %d\r\n",
							 PPM, g_measureCfg.measure_params.step_cnt_x[Axis_y], g_measureCfg.measure_params.pulse_cnt_x[Axis_y]);
		SG_LOG(SEVERITY_INFO, buf);

		snprintf(buf, buf_size, "PPM is %f and | CntPerCycle_z is %d | Pulses_z is %d\r\n",
							 PPM, g_measureCfg.measure_params.step_cnt_x[Axis_z], g_measureCfg.measure_params.pulse_cnt_x[Axis_y]);
		SG_LOG(SEVERITY_INFO, buf);

		free(buf);
	}
	else
	{

	}
	SG_LOG(SEVERITY_INFO, "Got Update Measure Config");

}





void tsk_Move_Execute(void *param)
{
	float p1,p2;
	uint32_t notify_value;
	FRESULT f_result;
//	adc_dma_init((uint32_t)&adc_buff[0]);
//	adc_dma_enable(ADC_BUF_LEN);

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

	f_result = Create_Measure_file();
	if(f_result == FR_OK)
	{
		SG_LOG(SEVERITY_INFO, "Create Measuring file success");
	}
	else
	{
		SG_LOG(SEVERITY_FATAL, "Create Measuring file failed");
	}

	while( tsk_WorkFlow_Handler() != true)
	{
		/*�������ֹͣ����*/
		notify_value = ulTaskNotifyTake(pdTRUE, 0);
		if(notify_value == NOTIFY_STOP_MEASURE)
		{
			SG_LOG(SEVERITY_WARNING, "Test stop due to STOP command");
			return ;
		}

		//vTaskDelay(100);
		//������ȣ����ڴ�ӡ���ȱ����ָ�
		#if ENABLE_UART_DRAW
			p1 = (float) (g_measureCfg.measure_params.x_index[Axis_x])/g_measureCfg.measure_params.step_cnt_x[Axis_x];
			p2 = (float) (g_measureCfg.measure_params.x_index[Axis_y])/g_measureCfg.measure_params.step_cnt_x[Axis_y];

			if( (g_measureCfg.measure_params.x_index[Axis_x] < g_measureCfg.measure_params.step_cnt_x[Axis_x]) &&
				(g_measureCfg.measure_params.x_index[Axis_y] < g_measureCfg.measure_params.step_cnt_x[Axis_y]) )
			{
				/*ѭ�����ⲿ��ͼ*/
				draw_rectangle(g_measureCfg.measure_params.dir_x[Axis_x],
							   g_measureCfg.measure_params.dir_x[Axis_y],
							   p1, p2);
				vTaskDelay(5);
				for(int i=0; i<10; i++)
				{
					printf("\033[1A"); //�Ȼص���һ��
				}
			}
		#endif
	}
	SG_LOG(SEVERITY_INFO, "Finish measure tracing task");

}



void tsk_Move_step(uint8_t axis_id, int set_point)
{
#if	1
	switch(axis_id)
	{
	case Axis_x:
		g_encoder_t.g_run_flag = 1;
		g_step_motor.setPoint_flag = true;
		if(g_measureCfg.measure_params.dir_x)
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



uint8_t s_workflow_table[Axis_Count]={
		//Axis_X			Axis_Y				Axis_Z
		Sub_WorkFlow_Y,	    Sub_WorkFlow_Z,		Sub_WorkFlow_Finish
};

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
	case 0:
	case 1:
	case 2:
		if(g_measureCfg.measure_params.x_index[*psub_flow] <=
		   g_measureCfg.measure_params.step_cnt_x[*psub_flow])
		{
			g_measureCfg.measure_params.x_index[*psub_flow]++;
#if !ENABLE_UART_DRAW
			tsk_Move_step(*psub_flow, g_measureCfg.measure_params.pulse_cnt_x[*psub_flow]);
#endif
			if(*psub_flow == Sub_WorkFlow_X)
				*pmain_flow = Main_WorkFlow_ADC;
			else if(*psub_flow == Sub_WorkFlow_Y)
				*psub_flow = Sub_WorkFlow_X;
			else if(*psub_flow == Sub_WorkFlow_Z)
				*psub_flow = Sub_WorkFlow_X;
		}
		else
		{
			g_measureCfg.measure_params.dir_x[*psub_flow] = !g_measureCfg.measure_params.dir_x[*psub_flow];
			char buf[30];
			snprintf(buf, 30, "Axis %d direction toggle", *psub_flow);
			SG_LOG(SEVERITY_INFO, buf);
			g_measureCfg.measure_params.x_index[*psub_flow] = 0;
			*psub_flow = s_workflow_table[*psub_flow];

		}
		break;


//	/*��Ϊ�����ⲿ��ͼ�����Ե�y_index = step_cnt_y��Y�ķ�ת�ж�ʱ�����ı�X�ķ��� */
//	case Sub_WorkFlow_X:
//		/*��ǰX���г�δ���*/
//		if(g_measureCfg.measure_params.x_index <= g_measureCfg.measure_params.step_cnt_x)
//		{
//			g_measureCfg.measure_params.x_index++;
//			//X��ջ��ƶ�
//#if !ENABLE_UART_DRAW
//			tsk_Move_step(Axis_x, g_measureCfg.measure_params.pulse_cnt_x);
//#endif
//			*pmain_flow = Main_WorkFlow_ADC;
//		}
//		else
//		{
////			if(g_measureCfg.y_index != g_measureCfg.step_cnt_y)
////			{
////				g_measureCfg.dir_x   = !g_measureCfg.dir_x;
////				printf("Axis X direction toggle\r\n");
////			}
//			g_measureCfg.measure_params.dir_x   = !g_measureCfg.measure_params.dir_x;
//			printf("Axis X direction toggle\r\n");
//			g_measureCfg.measure_params.x_index = 0;
//			*psub_flow = Sub_WorkFlow_Y;
//		}
//		break;
//
//	case Sub_WorkFlow_Y:
//		/*��ǰY���г�δ���*/
//		if(g_measureCfg.measure_params.y_index <= g_measureCfg.measure_params.step_cnt_y)
//		{
//			g_measureCfg.measure_params.y_index++;
//			//X��ջ��ƶ�
//			//tsk_Move_step(Axis_y, g_measureCfg.pulse_cnt_y);
//
//			*psub_flow = Sub_WorkFlow_X;
//		}
//		else
//		{
//			g_measureCfg.measure_params.dir_y   = !g_measureCfg.measure_params.dir_y;
//			g_measureCfg.measure_params.y_index = 0;
//			*psub_flow = Sub_WorkFlow_Z;
//		}
//		break;
//
//	case Sub_WorkFlow_Z:
//		/*��ǰY���г�δ���*/
//		if(g_measureCfg.measure_params.z_index < g_measureCfg.measure_params.step_cnt_z)
//		{
//			g_measureCfg.measure_params.z_index++;
//			//X��ջ��ƶ�
//			//tsk_Move_step(Axis_z, g_measureCfg.pulse_cnt_z);
//			*psub_flow = Sub_WorkFlow_X;
//		}
//		else
//		{
//			g_measureCfg.measure_params.z_index = 0;
//			*psub_flow = Sub_WorkFlow_Finish;
//		}
//		break;

	default:
		break;
	}
}


uint16_t adc_buff[ADC_BUF_LEN];
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
bool tsk_WorkFlow_Handler(void)
{
	static WorkFlow_Level_t s_workFlowLevel = {
			.MainFlow = Main_WorkFlow_Move,
			.SubFlow  = Sub_WorkFlow_X
	};


	assert(s_workFlowLevel.MainFlow>-1 && s_workFlowLevel.MainFlow < 3);
	if( NULL == sg_WorkFlow.workflow_handler.mainWorkFlow_api[s_workFlowLevel.MainFlow] )
	{
		initialize_WorkFlow_Api();
	}
	sg_WorkFlow.workflow_handler.mainWorkFlow_api[s_workFlowLevel.MainFlow](&s_workFlowLevel);


//	switch(s_workFlowLevel.MainFlow){
//	case Main_WorkFlow_Move:
//		Move_WorkFlow_handler(&s_workFlowLevel);
//		break;
//
//	case Main_WorkFlow_ADC:
//		ADC_WorkFlow_handler(&s_workFlowLevel);
//		break;
//
//	case Main_WorkFlow_Record:
//		Record_WorkFlow_handler(&s_workFlowLevel);
//		break;
//	}

	if(Sub_WorkFlow_Finish == s_workFlowLevel.SubFlow)
	{
		s_workFlowLevel.SubFlow  = Sub_WorkFlow_X;
		s_workFlowLevel.MainFlow = Main_WorkFlow_Move;
		return true;
	}
	else
		return false;
}

void tsk_USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
    uint32_t total, free;
    uint8_t res = 0;

    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_DISCONNECTION:
            f_mount(0, "0:", 1);        /* ж��U�� */
            printf("U Disk connecting...\r\n");
            break;

        case HOST_USER_CLASS_ACTIVE:
        	printf("U Disk connect successful!\r\n");
            res = f_mount(fs[0], "0:", 1);    /* ���¹���U�� */

            printf("mount fatfs return %d\r\n", res);

            res = exfuns_get_free("0:", &total, &free);

            if (res == 0)
            {
            	printf("FATFS OK!\r\n");
            	printf("U Disk Total Size:  %uMB\r\n", total >> 10);
            	printf("U Disk Free Size:   %uMB\r\n", free >> 10);

            	Create_Measure_file();
//            	uint8_t buf[50];
//            	time_string(buf, 50);
//            	mf_open(buf, FA_CREATE_ALWAYS);
//            	mf_scan_files("0:/MagnetMeasure");

            	vTaskDelay(500);
            	SG_LOG(SEVERITY_INFO, "Initialize U-disk Success");

            }
            else
            {
            	printf("FATFS ERROR!\r\n");
            }
            break;

        case HOST_USER_CONNECTION:
            break;

        default:
            break;
    }
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

//WorkFlow_Handler_t handler_api = {
//	.mainWorkFlow_api[Main_WorkFlow_Move] = Move_WorkFlow_handler,
//};


void initialize_WorkFlow_Api(void)
{
	sg_WorkFlow.workflow_level.MainFlow = Main_WorkFlow_Move;
	sg_WorkFlow.workflow_level.SubFlow  = Sub_WorkFlow_X;

	sg_WorkFlow.workflow_handler.mainWorkFlow_api[Main_WorkFlow_Move]   = &Move_WorkFlow_handler;
	sg_WorkFlow.workflow_handler.mainWorkFlow_api[Main_WorkFlow_ADC]    = &ADC_WorkFlow_handler;
	sg_WorkFlow.workflow_handler.mainWorkFlow_api[Main_WorkFlow_Record] = &Record_WorkFlow_handler;
	sg_WorkFlow.workflow_handler.subWorkFlow_api_moveX = NULL;
	//sg_WorkFlow.workflow_handler.
}

/**
 * Create_Measure_file
 * ���������ļ�, �ļ����м���ʱ���
 */
FRESULT Create_Measure_file(void)
{
	FRESULT f_result;
	/*���ȴ����ļ���,���Ѵ��ڸ��ļ��У��򴴽�ʧ��*/
	f_result = mf_mkdir(MEASURE_DIR);

	printf("Mkdir return %d\r\n", f_result);
	/*�ϳ��ļ���*/
	/* MEASURE_DIR/202210191450.SMTEST*/
	uint8_t file_name[50];
	time_string(file_name, 50);
	sprintf(file_name, "%s%s", file_name, ".SGTEST");
	f_result = mf_open(file_name, FA_CREATE_NEW | FA_WRITE | FA_READ);

	uint8_t buf_test_config[200];

	snprintf(buf_test_config ,"%s", "File description: This file is auto-generated by Scanning Magnet Test Stand and was just used in-house of Mevion Company\r\n\0");
	mf_write(buf_test_config, strlen(buf_test_config));

	snprintf(buf_test_config, 200,
			"Test Time: %s\r\n\0", file_name);
	mf_write(buf_test_config, strlen(buf_test_config));

	sprintf(buf_test_config, "Test Config:\r\n \
			 \t X_Step: %2.2fmm \r\n \
			 \t Y_Step: %2.2fmm \r\n \
			 \t Z_Step: %2.2fmm\0",
			 g_measureCfg.measure_config.step_x,
			 g_measureCfg.measure_config.step_y,
			 g_measureCfg.measure_config.step_z);

	mf_write(buf_test_config, strlen(buf_test_config));


	//free(buf_test_config);

	mf_close();
	return f_result;
}

