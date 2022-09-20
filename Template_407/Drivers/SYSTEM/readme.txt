    由正点原子提供的SYSTEM文件夹代码，用于快速构建工程，方便大家使用。
    1，delay文件夹：存放延时相关的驱动代码，支持在os下面使用。
    2，sys文件夹：存放系统相关驱动代码，包括系统时钟初始化，IO口配置，中断管理等三部分内容。
    3，usart文件夹：存放串口相关代码，支持printf，方便调试。
 
 

 ***************************************************************************************************
 * 广州市星翼电子科技有限公司(正点原子)
 * 电话号码: 020-38271790
 * 传真导码: 020-36773971
 * 购买地址: openedv.taobao.com
 * 在线视频: www.yuanzige.com
 * B 站视频: space.bilibili.com/394620890
 * 最新资料: www.openedv.com/forum.php/docs/index.html
 * 技术论坛: http://www.openedv.com/forum.php/forum.php
 * 公司网址: www.alientek.com
 *************************************************************************************************** 
 
 
 /*调试日志*/
 //8月23日，在F407模板工程上，添加FreeRTOS组件，初步调试PWM驱动步进电机成功
 //8月24日，开发串口通讯接收程序
 //8月25日，移植FreeRTOS，使用CubeIDE使能Middleware-freertos，但不使用CMSIS封装后的API
 //8月29日，与上位机软件调通接收Config命令；使用串口中断接收，发现在NVIC分组为3时，从中断里向消息队列发送消息
 //会引起任务终止。查询正点原子FreeRTOS开发手册，第四章中断配置和临界段中，明确讲述了FreeRTOS的配置中，没有处
 //理亚优先级的情况
 //8月30日，tStatistic任务，栈高水位线提示余量7字节，任务栈大小由128修改为256
 //8月30日，继续开发串口命令解析后的动作执行任务, 解析命令包中的浮点数成功
 //8月31日，等不及闭环步进电机到场了，先用TIM8的双通道输出不同相位的PWM信号，跳线接到TIM3的输入捕获通道，获取计数值
 //以此实现”模拟编码器“的功能。
 //9月7日，最近几天在调试定时器翻转模式输出90°相位差波形，用作模拟编码器测试;调试串口打印测试进度功能
 //BUG调试，程序进入硬件中断的原因是消息队列读写时，两端消息大小不一致造成; 工作流模式下串口打印工作进度时的错误，是
 //由于旧版函数是在3层for循环的内部打印，因此索引index不会等于step_cnt，而工作流模式下，会存在索引index等于step_cnt
 //的情况
 //9月8日，开始调试ADC功能; 规则序列，注入序列。F407处理器的ADC输入频率不能超过36MHz,如果超频，则会使ADC的转换结果准
 //确度下降。
 //正点原子ADC实验1测试成功，实验2测试成功
 //9月19日，调试编码器，目前读数不稳定, 调试OK。出现问题的原因是，计算好脉冲周期后，直接启动了定时器计数，应该由PID控制
 //环去启动定时器计数
 //9月20日，增加工作流ADC，Record功能函数;在板子上标记IO引脚的使用情况。
 //
 
55 00 00 01 05 50 AA
55 00 10 00 0a d7 23 3c 0a d7 23 3c 0a d7 23 3c 00 00 00 00 b1 86 aa



//	for(z=0; z<g_measureCfg.step_cnt_z; z++)
//	{
//		//z轴步进
//		for(y=0; y<g_measureCfg.step_cnt_y; y++)
//		{
//			//Y轴步进
//			for(x=0; x<g_measureCfg.step_cnt_x; x++)
//			{
//				cnt++;
//				/*检查有无停止命令*/
//				notify_value = ulTaskNotifyTake(pdTRUE, 0);
//				if(notify_value == NOTIFY_STOP_MEASURE)
//				{
//					printf("Test stop due to stop command \r\n");
//					return ;
//				}
//
//				//X轴步进
//				#if ENABLE_UART_DRAW
//					p1 = (float) (x)/g_measureCfg.step_cnt_x;
//					p2 = (float) (y)/g_measureCfg.step_cnt_y;
//					draw_rectangle(dir_x, dir_y, p1, p2);
//				#endif
//
//				//X轴闭环移动
//				tsk_Move_step(Axis_x, g_measureCfg.pulse_cnt_x);
//
//				/*ADC读取Hall数据*/
//
//				#if ENABLE_UART_DRAW
//					vTaskDelay(10);
//					if(cnt == 100)
//					{
//						cnt = 0;
//						overflow_times++;
//						printf("Current progress is %f\r\n", (float) overflow_times*100/
//															(g_measureCfg.step_cnt_x*
//															 g_measureCfg.step_cnt_y*
//															 g_measureCfg.step_cnt_z) );
//					}
//
//					for(int i=0; i<10; i++)
//					{
//						printf("\033[1A"); //先回到上一行
//					}
//				#endif
//			}
//			dir_x = !dir_x;
//			//Y轴闭环移动
//			tsk_Move_step(Axis_y, g_measureCfg.pulse_cnt_y);
//
//			#if ENABLE_UART_DRAW
//			//printf("Now axis_x direction %s \r\n", dir_x==true?"Forward ":"Backward");
//			//printf("\033[1A"); //先回到上一行
//			//fflush(stdout);
//			#endif
//		}
//		dir_y = !dir_y;
//
//		//Z轴闭环移动
//		//tsk_Move_step(Axis_z, g_measureCfg.pulse_cnt_z);
//
//		/*打印Y反向之前和之后都要换行*/
//		#if ENABLE_UART_DRAW
//		printf("\n");
//		printf("Now axis_y direction %s \r", dir_y==true?"Forward":"Backward");
//		printf("\n");
//		fflush(stdout);
//		#endif
//	}