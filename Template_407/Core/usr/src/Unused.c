/*旧版走位顺序打印函数*/
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
