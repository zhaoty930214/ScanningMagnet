    ������ԭ���ṩ��SYSTEM�ļ��д��룬���ڿ��ٹ������̣�������ʹ�á�
    1��delay�ļ��У������ʱ��ص��������룬֧����os����ʹ�á�
    2��sys�ļ��У����ϵͳ����������룬����ϵͳʱ�ӳ�ʼ����IO�����ã��жϹ�������������ݡ�
    3��usart�ļ��У���Ŵ�����ش��룬֧��printf��������ԡ�
 
 

 ***************************************************************************************************
 * ������������ӿƼ����޹�˾(����ԭ��)
 * �绰����: 020-38271790
 * ���浼��: 020-36773971
 * �����ַ: openedv.taobao.com
 * ������Ƶ: www.yuanzige.com
 * B վ��Ƶ: space.bilibili.com/394620890
 * ��������: www.openedv.com/forum.php/docs/index.html
 * ������̳: http://www.openedv.com/forum.php/forum.php
 * ��˾��ַ: www.alientek.com
 *************************************************************************************************** 
 
 
 /*������־*/
 //8��23�գ���F407ģ�幤���ϣ����FreeRTOS�������������PWM������������ɹ�
 //8��24�գ���������ͨѶ���ճ���
 //8��25�գ���ֲFreeRTOS��ʹ��CubeIDEʹ��Middleware-freertos������ʹ��CMSIS��װ���API
 //8��29�գ�����λ�������ͨ����Config���ʹ�ô����жϽ��գ�������NVIC����Ϊ3ʱ�����ж�������Ϣ���з�����Ϣ
 //������������ֹ����ѯ����ԭ��FreeRTOS�����ֲᣬ�������ж����ú��ٽ���У���ȷ������FreeRTOS�������У�û�д�
 //�������ȼ������
 //8��30�գ�tStatistic����ջ��ˮλ����ʾ����7�ֽڣ�����ջ��С��128�޸�Ϊ256
 //8��30�գ����������������������Ķ���ִ������, ����������еĸ������ɹ�
 //8��31�գ��Ȳ����ջ�������������ˣ�����TIM8��˫ͨ�������ͬ��λ��PWM�źţ����߽ӵ�TIM3�����벶��ͨ������ȡ����ֵ
 //�Դ�ʵ�֡�ģ����������Ĺ��ܡ�
 //9��7�գ���������ڵ��Զ�ʱ����תģʽ���90����λ��Σ�����ģ�����������;���Դ��ڴ�ӡ���Խ��ȹ���
 //BUG���ԣ��������Ӳ���жϵ�ԭ������Ϣ���ж�дʱ��������Ϣ��С��һ�����; ������ģʽ�´��ڴ�ӡ��������ʱ�Ĵ�����
 //���ھɰ溯������3��forѭ�����ڲ���ӡ���������index�������step_cnt����������ģʽ�£����������index����step_cnt
 //�����
 //9��8�գ���ʼ����ADC����; �������У�ע�����С�F407��������ADC����Ƶ�ʲ��ܳ���36MHz,�����Ƶ�����ʹADC��ת�����׼
 //ȷ���½���
 //����ԭ��ADCʵ��1���Գɹ���ʵ��2���Գɹ�
 //9��19�գ����Ա�������Ŀǰ�������ȶ�, ����OK�����������ԭ���ǣ�������������ں�ֱ�������˶�ʱ��������Ӧ����PID����
 //��ȥ������ʱ������
 //9��20�գ����ӹ�����ADC��Record���ܺ���;�ڰ����ϱ��IO���ŵ�ʹ�������
 //
 
55 00 00 01 05 50 AA
55 00 10 00 0a d7 23 3c 0a d7 23 3c 0a d7 23 3c 00 00 00 00 b1 86 aa



//	for(z=0; z<g_measureCfg.step_cnt_z; z++)
//	{
//		//z�Ჽ��
//		for(y=0; y<g_measureCfg.step_cnt_y; y++)
//		{
//			//Y�Ჽ��
//			for(x=0; x<g_measureCfg.step_cnt_x; x++)
//			{
//				cnt++;
//				/*�������ֹͣ����*/
//				notify_value = ulTaskNotifyTake(pdTRUE, 0);
//				if(notify_value == NOTIFY_STOP_MEASURE)
//				{
//					printf("Test stop due to stop command \r\n");
//					return ;
//				}
//
//				//X�Ჽ��
//				#if ENABLE_UART_DRAW
//					p1 = (float) (x)/g_measureCfg.step_cnt_x;
//					p2 = (float) (y)/g_measureCfg.step_cnt_y;
//					draw_rectangle(dir_x, dir_y, p1, p2);
//				#endif
//
//				//X��ջ��ƶ�
//				tsk_Move_step(Axis_x, g_measureCfg.pulse_cnt_x);
//
//				/*ADC��ȡHall����*/
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
//						printf("\033[1A"); //�Ȼص���һ��
//					}
//				#endif
//			}
//			dir_x = !dir_x;
//			//Y��ջ��ƶ�
//			tsk_Move_step(Axis_y, g_measureCfg.pulse_cnt_y);
//
//			#if ENABLE_UART_DRAW
//			//printf("Now axis_x direction %s \r\n", dir_x==true?"Forward ":"Backward");
//			//printf("\033[1A"); //�Ȼص���һ��
//			//fflush(stdout);
//			#endif
//		}
//		dir_y = !dir_y;
//
//		//Z��ջ��ƶ�
//		//tsk_Move_step(Axis_z, g_measureCfg.pulse_cnt_z);
//
//		/*��ӡY����֮ǰ��֮��Ҫ����*/
//		#if ENABLE_UART_DRAW
//		printf("\n");
//		printf("Now axis_y direction %s \r", dir_y==true?"Forward":"Backward");
//		printf("\n");
//		fflush(stdout);
//		#endif
//	}