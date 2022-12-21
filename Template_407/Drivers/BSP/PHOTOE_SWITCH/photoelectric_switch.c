#include "photoelectric_switch.h"
#include <stdio.h>


void SWITCH1_INT_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(OPTICAL_KEY_1_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(OPTICAL_KEY_1_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}


void SWITCH2_INT_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(OPTICAL_KEY_2_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(OPTICAL_KEY_2_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}


void SWITCH3_INT_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(OPTICAL_KEY_3_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(OPTICAL_KEY_3_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

void THERMAL_COUPLING_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(THERMAL_COUPLING_INTERLOCK_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(THERMAL_COUPLING_INTERLOCK_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

void TEMPER_FLOW_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(TEMPER_FLOW_INTERLOCK_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(TEMPER_FLOW_INTERLOCK_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case OPTICAL_KEY_1_PIN:
        	printf("Got EXTI1 interrupt\r\n");
            break;

        case OPTICAL_KEY_2_PIN:
        	printf("Got EXTI2 interrupt\r\n");
            break;

        case OPTICAL_KEY_3_PIN:
        	printf("Got EXTI3 interrupt\r\n");
            break;

        case THERMAL_COUPLING_INTERLOCK_PIN:
        	printf("Got Thermal Coupling interrupt\r\n");
			break;

        case TEMPER_FLOW_INTERLOCK_PIN:
        	printf("Got Temper Flow interrupt\r\n");
			break;

        default : break;
    }
}


void init_input_io(void)
{
	GPIO_InitTypeDef gpio_init_struct;                 /* GPIO配置参数存储变量 */
	SWITCH1_GPIO_CLK_ENABLE();   				  	 /* 端口时钟使能 */
	SWITCH2_GPIO_CLK_ENABLE();
	SWITCH3_GPIO_CLK_ENABLE();
	TEMPER_FLOW_GPIO_CLK_ENABLE();
	THERMAL_COUPLING_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = OPTICAL_KEY_1_PIN;                /* 光电开关1引脚 */
	gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 输入 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;            /* 高速 */
	gpio_init_struct.Pull = GPIO_PULLUP;     		   		 /* 上拉 */
	HAL_GPIO_Init(OPTICAL_KEY_1_PORT, &gpio_init_struct);    /* 光电开关1引脚模式设置,输入 */

	gpio_init_struct.Pin = OPTICAL_KEY_2_PIN;                /* 光电开关2引脚 */
	HAL_GPIO_Init(OPTICAL_KEY_2_PORT, &gpio_init_struct);    /* 光电开关2引脚模式设置,输入 */

	gpio_init_struct.Pin = OPTICAL_KEY_3_PIN;                /* 光电开关3引脚 */
	HAL_GPIO_Init(OPTICAL_KEY_3_PORT, &gpio_init_struct);    /* 光电开关3引脚模式设置,输入 */

	/*热耦合联锁外部中断*/
	gpio_init_struct.Pin = THERMAL_COUPLING_INTERLOCK_PIN;                /* 光电开关2引脚 */
	HAL_GPIO_Init(THERMAL_COUPLING_INTERLOCK_PORT, &gpio_init_struct);    /* 光电开关2引脚模式设置,输入 */


	/*温度流量联锁外部中断*/
	gpio_init_struct.Pin = TEMPER_FLOW_INTERLOCK_PIN;                /* 光电开关2引脚 */
	HAL_GPIO_Init(TEMPER_FLOW_INTERLOCK_PORT, &gpio_init_struct);    /* 光电开关2引脚模式设置,输入 */

	/********************光电开关外部中断初始化**************************/
    HAL_NVIC_SetPriority(SWITCH1_INT_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(SWITCH1_INT_IRQn);                       /* 使能中断线0 */

    HAL_NVIC_SetPriority(SWITCH2_INT_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(SWITCH2_INT_IRQn);                       /* 使能中断线4 */

    HAL_NVIC_SetPriority(SWITCH3_INT_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(SWITCH3_INT_IRQn);                       /* 使能中断线15_10 */

    HAL_NVIC_SetPriority(TEMPER_FLOW_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(TEMPER_FLOW_IRQn);                       /* 使能中断线3 */

    HAL_NVIC_SetPriority(THERMAL_COUPLING_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(THERMAL_COUPLING_IRQn);                       /* 使能中断线9_5 */
	/********************光电开关外部中断初始化**************************/
}


void init_photoelectric_switch()
{
	init_input_io();
}
