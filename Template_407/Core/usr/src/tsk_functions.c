#include "tsk_functions.h"
#include "Measure_Constants.h"
#include "LED.h"
#include "stepper_tim.h"
#include "gtim.h"
#include "adc.h"
#include "rtc.h"

extern MeasureConfig_t g_measureCfg;

/*
 * 工作流处理结构体
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
 * 软件定时器句柄
 */
TimerHandle_t	AutoReloadTimer_Handle;

/**
 * 消息队列句柄
 */
xQueueHandle	Queue_Usart;
xQueueHandle	Queue_Measure;
xQueueHandle	Queue_MotorReady;

/*
 * 软件定时器回调
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
	//创建软件周期定时器, 周期定时器，周期100ms(100 个时钟节拍),周期模式
	AutoReloadTimer_Handle=xTimerCreate((const char*            )"AutoReloadTimer",
										(TickType_t             )100,
										(UBaseType_t            )pdTRUE,
										(void*)1,
										(TimerCallbackFunction_t)AutoReloadCallback);

	//创建消息队列
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

	/*电机就位，只允许写入1个长度，X&&Y&&Z均就位时才能从PID中断里向消息队列写入*/
	Queue_MotorReady = xQueueCreate(MotorReady_QUEUE_LENGTH, QUEUE_SIZE);
	if(Queue_MotorReady == NULL)
	{
		printf("Create Queue_MotorReady\r\n");
	}
}

/*初始化用户调节的目标（对应哪个环的目标）*/
float *MOTOR_X_GO = (float*)(&g_location_pid.SetPoint);
float *MOTOR_Y_GO = (float*)(&g_location_pid.SetPoint);
float *MOTOR_Z_GO = (float*)(&g_location_pid.SetPoint);

bool tsk_WaitMoveComplete(uint8_t axis_id)
{
	bool ret;
	int queue_recv_item;
	//阻塞等待,电机运动完成
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
 * 简单的串口输出画矩形函数
 */
void draw_rectangle(bool dir_x, bool dir_y, float x_proportion, float y_proportion)
{

	int i;
	int j; //(dir_y == CW)? 0:1;
	int y_limit = y_proportion * Y_Items;
	int x_limit = x_proportion * X_Items;


	//从下向上运动时，y_limit也是从下开始数，只能输出0~9，
	//则cmp_obj的值则为10-0~9 = 1~10,
	//此时j遍历范围0~9显然不合理， 所以设置j从1开始
	int cmp_obj = (dir_y == CW)?y_limit:Y_Items-y_limit-1;

//	char s_finish[10]="**********";
//	char s_unfinish[10]="----------";
	switch(dir_y)
	{
		case CW:					//Y步进方向与画图方向一致
		case CCW:
		{
			//逐行遍历
			for(j=0; j<Y_Items; j++)
			{
//				if(j == 0)
//					printf("Current X dir is %c\r\n", (g_measureCfg.dir_x==CW)?'+':'-');
				//由上至下运动时，比较目标为已完成区域;反之为未完成区域
				if(j<cmp_obj)
				{
					//画完一行时，需要换行
					if(dir_y == CW)
						printf("**********\r\n");
					else
						printf("----------\r\n");
					continue;
				}
				else if(j> cmp_obj)
				{
					//画完一行时，需要换行
					if(dir_y == CW)
						printf("----------\r\n");
					else
						printf("**********\r\n");
					continue;
				}
				//找到当前正在运动的一行，逐点遍历
				else if(j==cmp_obj)
				{
					for(i=0; i<(X_Items); i++)
					{
						//由左至右运动时，比较目标为已完成区域;反之为未完成区域
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
					//画完一行时，需要换行
					printf("\r\n");
				}
			}
		}
		break;
	}
}


/**
 * 初始化XYZ三轴的默认单次距离，单位mm
 * 输入参数 @x, @y, @z 各轴的默认单次移动距离，有效范围0~10mm，小数点后2位有效
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
	/*清零XYZ计数值*/
	g_measureCfg.measure_params.x_index[Axis_x] = 0;
	g_measureCfg.measure_params.x_index[Axis_y] = 0;
	g_measureCfg.measure_params.x_index[Axis_z] = 0;

	g_measureCfg.measure_params.dir_x[Axis_x] = AXIS_X_DEFAULT_DIRECTION;
	g_measureCfg.measure_params.dir_x[Axis_y] = AXIS_Y_DEFAULT_DIRECTION;

	/*计算三轴各自的步进计次数*/
	g_measureCfg.measure_params.step_cnt_x[Axis_x] = X_LENGTH/g_measureCfg.measure_config.step_x;
	g_measureCfg.measure_params.step_cnt_x[Axis_y] = Y_LENGTH/g_measureCfg.measure_config.step_y;
	g_measureCfg.measure_params.step_cnt_x[Axis_z] = Z_LENGTH/g_measureCfg.measure_config.step_z;

	/*计算三轴各自的步进脉冲数 = 每毫米对应的脉冲数*步进距离毫米单位. */
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

	/*开始测试前，先回归原点P0， 伪代码*/
	//go_to_origin();

	/*回归原点P0后，寻找测试锚点，即磁铁孔径的相对原点P1*/
	//go_to_relative_origin();

	/*根据需要，看是否添加人员确认P1点走位*/
	//xQueueReceive(Queue_rltOrigin_confirm, &bd, portDelayMax);
	//

	/*一边走位，一边向串口控制台发送路径图形,发到上位机，这个功能牛逼，必须安排上。
	*磁铁孔径区是一个圆柱形区域，其位置大致如图. 考虑到节能减排和省电的考量，
	*不让电机在每个XY平面测量完成时重新回到当前平面的相对原点，而是采用蛇形走位，
	*因此，Z轴方向固定不变，X和Y轴每次走到各自的尽头后，需要反向。			*/
	/*  .
	 * ...
	 *.....
	 * ...
	 *  .   */

	/*测试流程*/
	//make_moving_plan;
	//measuring module
	//step
	//get hall data

	/*开始测量前，清除通知值*/
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
		/*检查有无停止命令*/
		notify_value = ulTaskNotifyTake(pdTRUE, 0);
		if(notify_value == NOTIFY_STOP_MEASURE)
		{
			SG_LOG(SEVERITY_WARNING, "Test stop due to STOP command");
			return ;
		}

		//vTaskDelay(100);
		//计算进度，串口打印进度表或发送指令。
		#if ENABLE_UART_DRAW
			p1 = (float) (g_measureCfg.measure_params.x_index[Axis_x])/g_measureCfg.measure_params.step_cnt_x[Axis_x];
			p2 = (float) (g_measureCfg.measure_params.x_index[Axis_y])/g_measureCfg.measure_params.step_cnt_x[Axis_y];

			if( (g_measureCfg.measure_params.x_index[Axis_x] < g_measureCfg.measure_params.step_cnt_x[Axis_x]) &&
				(g_measureCfg.measure_params.x_index[Axis_y] < g_measureCfg.measure_params.step_cnt_x[Axis_y]) )
			{
				/*循环体外部作图*/
				draw_rectangle(g_measureCfg.measure_params.dir_x[Axis_x],
							   g_measureCfg.measure_params.dir_x[Axis_y],
							   p1, p2);
				vTaskDelay(5);
				for(int i=0; i<10; i++)
				{
					printf("\033[1A"); //先回到上一行
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
 *三轴移动函数
 *@参数, param: 工作流结构体指针参数
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


//	/*因为是在外部作图，所以当y_index = step_cnt_y即Y的翻转判定时，不改变X的方向 */
//	case Sub_WorkFlow_X:
//		/*当前X轴行程未完成*/
//		if(g_measureCfg.measure_params.x_index <= g_measureCfg.measure_params.step_cnt_x)
//		{
//			g_measureCfg.measure_params.x_index++;
//			//X轴闭环移动
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
//		/*当前Y轴行程未完成*/
//		if(g_measureCfg.measure_params.y_index <= g_measureCfg.measure_params.step_cnt_y)
//		{
//			g_measureCfg.measure_params.y_index++;
//			//X轴闭环移动
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
//		/*当前Y轴行程未完成*/
//		if(g_measureCfg.measure_params.z_index < g_measureCfg.measure_params.step_cnt_z)
//		{
//			g_measureCfg.measure_params.z_index++;
//			//X轴闭环移动
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

	/*ADC采样*/
	pWorkFlow->MainFlow = Main_WorkFlow_Record;

}

void Record_WorkFlow_handler(WorkFlow_Level_t *pWorkFlow)
{
	/*数据记录*/
	pWorkFlow->MainFlow = Main_WorkFlow_Move;


}

/**
 * 测量任务开始
 * 1.获取当前主工作状态.
 * 2.主工作状态，Main_WorkFlow_Move-> Main_WorkFlow_ADC ->Main_WorkFlow_Move...
 * 3.子工作状态，Main_WorkFlow_Move-> Sub_WorkFlow_X
 * 							   -> Sub_WorkFlow_Y
 * 							   -> Sub_WorkFlow_Z
 * 4.主状态转换，Move -> ADC
 * 5.子状态转换
 *   X,Y,Z是否运动到坐标轴边缘？
 *		X轴边缘 -> 子状态改为	 Y轴
 *		Y轴边缘 -> 子状态改为 Z轴
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
            f_mount(0, "0:", 1);        /* 卸载U盘 */
            printf("U Disk connecting...\r\n");
            break;

        case HOST_USER_CLASS_ACTIVE:
        	printf("U Disk connect successful!\r\n");
            res = f_mount(fs[0], "0:", 1);    /* 重新挂载U盘 */

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


//电机带动滑轨移动1mm，对应的脉冲数
#define PULSE_PER_MM	50
extern  volatile uint16_t g_step_angle;                         /* 设置的步进角度 */

#define SETPOINT_PULSE_PER_MM	ENCODER_SPR/MPR

int g_pulse_count = 0;
void sliede_way_test(uint32_t dis_mm, bool dir_t)
{
	/*移动距离= pulse per milimeter*milimeter.*/

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
//		while(g_step_angle > 0);	/*持续至当前方向走完*/
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
 * 创建测试文件, 文件名中加入时间戳
 */
FRESULT Create_Measure_file(void)
{
	FRESULT f_result;
	/*首先创建文件夹,若已存在该文件夹，则创建失败*/
	f_result = mf_mkdir(MEASURE_DIR);

	printf("Mkdir return %d\r\n", f_result);
	/*合成文件名*/
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

