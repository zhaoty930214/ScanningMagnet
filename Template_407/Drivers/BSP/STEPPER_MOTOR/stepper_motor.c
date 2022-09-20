/**
 ****************************************************************************************************
 * @file        stepper_motor.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       ������� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#include "stepper_motor.h"
#include "stepper_tim.h"
#include "atim.h"
#include "gtim.h"
#include "main.h"
#include "my_tasks.h"
#include <stdio.h>
#include <stdbool.h>
/**
 * @brief       ��ʼ������������IO��, ��ʹ��ʱ��
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void stepper_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;

    STEPPER_DIR1_GPIO_CLK_ENABLE();                                 /* DIR1ʱ��ʹ�� */
    STEPPER_DIR2_GPIO_CLK_ENABLE();                                 /* DIR2ʱ��ʹ�� */
    STEPPER_DIR3_GPIO_CLK_ENABLE();                                 /* DIR3ʱ��ʹ�� */
    STEPPER_DIR4_GPIO_CLK_ENABLE();                                 /* DIR4ʱ��ʹ�� */
            
    STEPPER_EN1_GPIO_CLK_ENABLE();                                  /* EN1ʱ��ʹ�� */
    STEPPER_EN2_GPIO_CLK_ENABLE();                                  /* EN2ʱ��ʹ�� */
    STEPPER_EN3_GPIO_CLK_ENABLE();                                  /* EN3ʱ��ʹ�� */
    STEPPER_EN4_GPIO_CLK_ENABLE();                                  /* EN4ʱ��ʹ�� */
    

    gpio_init_struct.Pin = STEPPER_DIR1_GPIO_PIN;                   /* DIR1���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;                    /* ������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                          /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;                   /* ���� */
    HAL_GPIO_Init(STEPPER_DIR1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��DIR1���� */

    gpio_init_struct.Pin = STEPPER_DIR2_GPIO_PIN;                   /* DIR2���� */
    HAL_GPIO_Init(STEPPER_DIR2_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��DIR2���� */

    gpio_init_struct.Pin = STEPPER_DIR3_GPIO_PIN;                   /* DIR3���� */
    HAL_GPIO_Init(STEPPER_DIR3_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��DIR3���� */

    gpio_init_struct.Pin = STEPPER_DIR4_GPIO_PIN;                   /* DIR4���� */
    HAL_GPIO_Init(STEPPER_DIR4_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��DIR4���� */
    
    /*   �ѻ����ų�ʼ��   */
    
    gpio_init_struct.Pin = STEPPER_EN1_GPIO_PIN;                    /* EN1���� */
    HAL_GPIO_Init(STEPPER_EN1_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��EN1���� */
    
    gpio_init_struct.Pin = STEPPER_EN2_GPIO_PIN;                    /* EN2���� */
    HAL_GPIO_Init(STEPPER_EN2_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��EN2���� */
    
    gpio_init_struct.Pin = STEPPER_EN3_GPIO_PIN;                    /* EN3���� */
    HAL_GPIO_Init(STEPPER_EN3_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��EN3���� */
    
    gpio_init_struct.Pin = STEPPER_EN4_GPIO_PIN;                    /* EN4���� */
    HAL_GPIO_Init(STEPPER_EN4_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��EN4���� */
    
    atim_timx_oc_chy_init(arr, psc);                                /* ��ʼ��PUL���ţ��Լ�����ģʽ�� */
}


/**
 * @brief       �����������
 * @param       motor_num: ��������ӿ����
 * @retval      ��
 */
void stepper_star(uint8_t motor_num)
{
    switch(motor_num)
    {
        /* ������ӦPWMͨ�� */
        case STEPPER_MOTOR_1 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
            	g_encoder_t.g_run_flag = 1;
                HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);
            }
            break;
        }
        case STEPPER_MOTOR_2 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH2);
            }
            break;
        }
        case STEPPER_MOTOR_3 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM1||g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Start(&g_atimx_handle, ATIM_TIMX_PWM_CH3);
            }
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH3);
            }
            break;  
        }
        case STEPPER_MOTOR_4 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM1||g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Start(&g_atimx_handle, ATIM_TIMX_PWM_CH4);
            }
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Start_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH4);
            }
            break;
        }
        default : break;
    }
}

/**
 * @brief       �رղ������
 * @param       motor_num: ��������ӿ����
 * @retval      ��
 */
void stepper_stop(uint8_t motor_num)
{
    switch(motor_num)
    {
        case STEPPER_MOTOR_1 :
        {
            /* �رն�ӦPWMͨ�� */
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM1||g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Stop(&g_atimx_handle, ATIM_TIMX_PWM_CH1);
            }
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);
            }
            break;
        }
        case STEPPER_MOTOR_2 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM1||g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Stop(&g_atimx_handle, ATIM_TIMX_PWM_CH2);
            }
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH2);
            }
            break;
        }
        case STEPPER_MOTOR_3 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM1||g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Stop(&g_atimx_handle, ATIM_TIMX_PWM_CH3);
            }
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH3);
            }
            break;  
        }
        case STEPPER_MOTOR_4 :
        {
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM1||g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Stop(&g_atimx_handle, ATIM_TIMX_PWM_CH4);
            }
            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH4);
            }
            break;
        }
        default : break;
    }
}

/********************************** ����������ƴ��� *****************************************/
/*ȫ�ֱ���*/

Motor_TypeDef  g_step_motor;                                    /* ����������� */
volatile uint16_t g_step_angle = 65535;                         /* ���õĲ����Ƕ� */

/**
 * @brief: �������λ���˶�����,ע��˺���ֻ�ڶ�ʱ���жϷ������б�����
 * @param: dir:��������˶����� 0:��ת 1��ת
           location_m: PID���������ֵ��������ֵΪ��������ֵ��
 * @retval ��
 */
void stepper_motion_ctrl(uint8_t dir, uint16_t location_m)      /* location_m������ֵ����100���Ǿ���20ms�Ĳ���Ƶ����Ҫ���100�������ź� */
{
    float step_delay = 0.0;                                     /* ������ʱ */
    static int debug_out = 0;
    int queue_item;
    if(location_m == 0)
    {
        HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);  /* ������ֵ��Ŀ��ֵһ��ʱ�������ѵ�ָ��λ�ã�ֹͣ��� */

        g_step_angle = 0;                                        /* ������� */
        if(debug_out == 0)
        {
        	debug_out = 1;
            //printf("Motor has rotated to special location!\r\n");
            //printf("Current encoder value is %d\r\n", gtim_get_encode());
            //���ж�������з�����Ϣ,��Ҫ����3����ıջ�����ID��������ʱд�̶�0x11
            queue_item = 0x11;
            xQueueSendFromISR(Queue_MotorReady, &queue_item, false);
        }
    }
    else
    {
    	/*clear debug_out recortd */
    	debug_out = 0;

        if(dir == CW)                                           /* ������ת���� */
        	ST1_DIR(CW);
        else
        	ST1_DIR(CCW);

        /* ����PID����õ��Ľ���Ǳ��������������ֵ,
         * location_m * FEEDBACK_CONST = ������������ֵת��֮��Ĳ������������
         * step_dealy = 20ms/location_m * FEEDBACK_CONST�� ����ڲ������ÿ��������������Ӧ��ʱ�䣬��λΪms
          ����ת��Ϊ�������������������� ����������ֵ * ���������һȦ���������� / ������һȦ����ֵ����*/
        step_delay = (float)(SMAPLSE_PID_PERIOD /(location_m * FEEDBACK_CONST));   /* ���������ʱ���ȣ���λ�� ms */

        /* msת��s step_delay = step_delay/1000 */
        /*
           T : �������������ʱ��
           c : ��Ҫ���ıȽ�ֵ
           f : ��ʱ������Ƶ��
           T  = c *(1/f)
           step_delay/1000 = c*1/1000000 (��λ��s)
           c = step_delay*1000   �����һ������ļ���ֵ����Ҫ����2 ��Ϊһ��������������Ҫ��ת2��
           ��Ҫ�ļ���ֵ��c/2 = Sstep_delay*1000/2 = step_delay*500
        */
        /*ģ���������֧*/
        g_step_angle = (uint16_t)(step_delay * 500);            /* ������Ľ��������,�������2,�����ڷ�תһ�� */

        //g_step_angle = location_m * FEEDBACK_CONST;

        //printf("We want another cnt %d\r\n", g_step_angle);
        HAL_TIM_OC_Start_IT(&g_atimx_handle,ATIM_TIMX_PWM_CH1);
        //HAL_TIM_OC_Start_IT(&g_atimx_handle,ATIM_TIMX_PWM_CH2);
    }
}

