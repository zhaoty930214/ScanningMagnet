#include "photoelectric_switch.h"
#include <stdio.h>


void SWITCH1_INT_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(OPTICAL_KEY_1_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(OPTICAL_KEY_1_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}


void SWITCH2_INT_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(OPTICAL_KEY_2_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(OPTICAL_KEY_2_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}


void SWITCH3_INT_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(OPTICAL_KEY_3_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(OPTICAL_KEY_3_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

void THERMAL_COUPLING_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(THERMAL_COUPLING_INTERLOCK_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(THERMAL_COUPLING_INTERLOCK_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

void TEMPER_FLOW_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(TEMPER_FLOW_INTERLOCK_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(TEMPER_FLOW_INTERLOCK_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */

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
	GPIO_InitTypeDef gpio_init_struct;                 /* GPIO���ò����洢���� */
	SWITCH1_GPIO_CLK_ENABLE();   				  	 /* �˿�ʱ��ʹ�� */
	SWITCH2_GPIO_CLK_ENABLE();
	SWITCH3_GPIO_CLK_ENABLE();
	TEMPER_FLOW_GPIO_CLK_ENABLE();
	THERMAL_COUPLING_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = OPTICAL_KEY_1_PIN;                /* ��翪��1���� */
	gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* ���� */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;            /* ���� */
	gpio_init_struct.Pull = GPIO_PULLUP;     		   		 /* ���� */
	HAL_GPIO_Init(OPTICAL_KEY_1_PORT, &gpio_init_struct);    /* ��翪��1����ģʽ����,���� */

	gpio_init_struct.Pin = OPTICAL_KEY_2_PIN;                /* ��翪��2���� */
	HAL_GPIO_Init(OPTICAL_KEY_2_PORT, &gpio_init_struct);    /* ��翪��2����ģʽ����,���� */

	gpio_init_struct.Pin = OPTICAL_KEY_3_PIN;                /* ��翪��3���� */
	HAL_GPIO_Init(OPTICAL_KEY_3_PORT, &gpio_init_struct);    /* ��翪��3����ģʽ����,���� */

	/*����������ⲿ�ж�*/
	gpio_init_struct.Pin = THERMAL_COUPLING_INTERLOCK_PIN;                /* ��翪��2���� */
	HAL_GPIO_Init(THERMAL_COUPLING_INTERLOCK_PORT, &gpio_init_struct);    /* ��翪��2����ģʽ����,���� */


	/*�¶����������ⲿ�ж�*/
	gpio_init_struct.Pin = TEMPER_FLOW_INTERLOCK_PIN;                /* ��翪��2���� */
	HAL_GPIO_Init(TEMPER_FLOW_INTERLOCK_PORT, &gpio_init_struct);    /* ��翪��2����ģʽ����,���� */

	/********************��翪���ⲿ�жϳ�ʼ��**************************/
    HAL_NVIC_SetPriority(SWITCH1_INT_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(SWITCH1_INT_IRQn);                       /* ʹ���ж���0 */

    HAL_NVIC_SetPriority(SWITCH2_INT_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(SWITCH2_INT_IRQn);                       /* ʹ���ж���4 */

    HAL_NVIC_SetPriority(SWITCH3_INT_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(SWITCH3_INT_IRQn);                       /* ʹ���ж���15_10 */

    HAL_NVIC_SetPriority(TEMPER_FLOW_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(TEMPER_FLOW_IRQn);                       /* ʹ���ж���3 */

    HAL_NVIC_SetPriority(THERMAL_COUPLING_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(THERMAL_COUPLING_IRQn);                       /* ʹ���ж���9_5 */
	/********************��翪���ⲿ�жϳ�ʼ��**************************/
}


void init_photoelectric_switch()
{
	init_input_io();
}
