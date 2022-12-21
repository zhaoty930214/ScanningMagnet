/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "my_usart.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "stepper_motor.h"
#include "KEY.h"
#include "uart_commu.h"
#include "my_tasks.h"
#include "rs232.h"
#include "gtim.h"
#include "btim.h"
#include "pid.h"
#include "adc.h"
#include <stdio.h>
#include "exfuns.h"
#include "photoelectric_switch.h"
#include "usbh_conf.h"
#include "rtc.h"

void MX_FREERTOS_Init(void);

/*闭环模式，指定脉冲输出数*/
int angle = 90;
int pulse_cnt =0;

TaskHandle_t Start_Handle_t;



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */
	/* USER CODE END 1 */
	uint8_t hour, min, sec, milisec, ampm;

	HAL_Init();
	//MX_GPIO_Init();
	sys_stm32_clock_init(336, 8, 2, 7);
	delay_init(168);
	led_init();
	key_init();                              /* 初始化按键 */
	usart_init(115200);


	//rs232_init(9600);                       /* 初始化RS232 */

	debug_init();
	stepper_init(0xFFFF-1, 84 - 1);      		/*初始化步进电机驱动和TIM8		   */
	gtim_timx_encoder_chy_init(0xffff, 0);		/*初始化TIM3(编码器模式)，采样编码器*/
	btim_timx_int_init(1000-1, 84-1);			/*初始化TIM6普通定时器，20ms周期闭环检测*/

	/*MCU测试函数*/
	debug_structSize();
	pid_init();
	init_photoelectric_switch();
	init_input_io();

	rtc_init();
    rtc_get_time(&hour, &min, &sec, &milisec, &ampm);
	printf("Time:%02d:%02d:%02d\r\n", hour, min, sec);

	#ifdef ENCODER_SIMULATOR_MODE
	//  stepper_star(STEPPER_MOTOR_1);
	//  stepper_star(STEPPER_MOTOR_2);
	#endif


	/*文件系统和USB 驱动相关*/
	exfuns_init();		//为Fatfs相关变量申请内存

	USB_Init();

	//rs232_send_data((uint8_t *)("\r\n hello world\r\n \0"), 20);
	xTaskCreate(start_task,
			  START_TASK_NAME,
			  START_Task_Stack,
			  NULL,
			  START_Task_Prio,
			  &Start_Handle_t);

	vTaskStartScheduler();
	while(1)
	{
	  //USBH_Process(&g_hUSBHost);
	  delay_ms(10);
	}
}





#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
