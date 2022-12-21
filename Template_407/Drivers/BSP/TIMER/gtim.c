/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       ͨ�ö�ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211015
 * ��һ�η���
 * V1.1 20211015
 * ����gtim_timx_pwm_chy_init����
 ****************************************************************************************************
 */

#include "gtim.h"
#include "led.h"
//#include "pid.h"
#include "stdlib.h"
#include "stepper_motor.h"
#include "btim.h"
#include "pid.h"
#include <stdio.h>
#include <stdbool.h>
/******************************************************************************************/

/* ��ʱ�����þ�� ���� */
TIM_HandleTypeDef g_timx_handle;             /* ��ʱ��x��� */

/*��ʱ��PWM*/
static TIM_HandleTypeDef g_timx_encode_chy_handle;     /* ��ʱ��x��� */
//TIM_OC_InitTypeDef g_timx_oc_pwm_chy_handle; /* ��ʱ�������� */
static TIM_Encoder_InitTypeDef g_timx_encoder_chy_handle; /* ��ʱ����������� */
//uint8_t g_timxchy_cap_sta = 0;               /* ���벶��״̬ */
//uint16_t g_timxchy_cap_val = 0;              /* ���벶��ֵ */


/*---------------------------��������ز�����ȫ�ֱ���---------------------------*/
Encoder_t g_encoder_t;

/******************************************************************************************/


//volatile int g_timx_encode_count = 0;

/**
 * @brief       ͨ�ö�ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ42M, ���Զ�ʱ��ʱ�� = 84Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE(); /* ʹ��TIMxʱ�� */

    g_timx_handle.Instance = GTIM_TIMX_INT;                     /* ͨ�ö�ʱ��x */
    g_timx_handle.Init.Prescaler = psc;                         /* ��Ƶ */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ���ϼ����� */
    g_timx_handle.Init.Period = arr;                            /* �Զ�װ��ֵ */
    g_timx_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_Base_Init(&g_timx_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 3);             /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                     /* ����ITMx�ж� */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                      /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_encode_chy_handle);
}

/*********************************ͨ�ö�ʱ��encoder����*************************************/
/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y �������ӿ�ģʽ ��ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ42M, ���Զ�ʱ��ʱ�� = 84Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_encoder_chy_init(uint16_t arr, uint16_t psc)
{
    g_timx_encode_chy_handle.Instance = GTIM_TIMX_ENCODER;                 /* ��ʱ��x */
    g_timx_encode_chy_handle.Init.Prescaler = psc;                         /* ��ʱ����Ƶ */
    g_timx_encode_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ���ϼ���ģʽ */
    g_timx_encode_chy_handle.Init.Period = arr;                            /* �Զ���װ��ֵ */
    g_timx_encode_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  /* ʱ�ӷ�Ƶ���� */
    g_timx_encoder_chy_handle.EncoderMode = TIM_ENCODERMODE_TI12;          /* TI1,TI2����� */
    g_timx_encoder_chy_handle.IC1Polarity = TIM_ICPOLARITY_RISING;         /* ���뼫�ԣ��Ƿ��� */
    g_timx_encoder_chy_handle.IC1Selection = TIM_ICSELECTION_DIRECTTI;     /* ����ͨ��ѡ�� */
    g_timx_encoder_chy_handle.IC1Prescaler = TIM_ICPSC_DIV1;               /* һ��Ƶ */
    g_timx_encoder_chy_handle.IC1Filter = 10;                              /* �˲������� */
    g_timx_encoder_chy_handle.IC2Polarity = TIM_ICPOLARITY_RISING;         /* ���뼫�ԣ��Ƿ��� */
    g_timx_encoder_chy_handle.IC2Selection = TIM_ICSELECTION_DIRECTTI;     /* ����ͨ��ѡ�� */
    g_timx_encoder_chy_handle.IC2Prescaler = TIM_ICPSC_DIV1;               /* һ��Ƶ */
    g_timx_encoder_chy_handle.IC2Filter = 10;                              /* �˲������� */
    HAL_TIM_Encoder_Init(&g_timx_encode_chy_handle, &g_timx_encoder_chy_handle);

    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle,GTIM_TIMX_ENCODER_CH1);
    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle,GTIM_TIMX_ENCODER_CH2);
    __HAL_TIM_CLEAR_FLAG(&g_timx_encode_chy_handle,TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&g_timx_encode_chy_handle,TIM_IT_UPDATE);

}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_Encoder_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *tim_encoderHandle)
{
    if (tim_encoderHandle->Instance == GTIM_TIMX_ENCODER)
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE();                            /* ����ͨ��y��CPIOʱ�� */
        GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE();
        GTIM_TIMX_ENCODER_CH1_CLK_ENABLE();                                 /* ������ʱ��ʱ�� */
        GTIM_TIMX_ENCODER_CH2_CLK_ENABLE();

        gpio_init_struct.Pin = GTIM_TIMX_ENCODER_CH1_GPIO_PIN;              /* ͨ��y��CPIO�� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_struct.Pull = GPIO_NOPULL;                            /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;                  /* ���� */
        gpio_init_struct.Alternate = GTIM_TIMX_ENCODERCH1_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIMX_ENCODER_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = GTIM_TIMX_ENCODER_CH2_GPIO_PIN;              /* ͨ��y��CPIO�� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_struct.Pull = GPIO_NOPULL;                            /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;                  /* ���� */
        gpio_init_struct.Alternate = GTIM_TIMX_ENCODERCH2_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIMX_ENCODER_CH2_GPIO_PORT, &gpio_init_struct);

        //��ʱ��������ģʽ���ж����ȼ���Ӧ�ø���PID�������ȼ�����ʱ��������жϴ���
        HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 15, 0);				//NVIC�жϷ���ΪGroup4���ж����ȼ�����Ϊ5������PWM�жϣ�����PID�����ж�
        HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);

    }
}

/**
 * @brief       ��ȡ��������ֵ
 * @param       ��
 * @retval      ������ֵ
 */
int gtim_get_encode(void)
{
    return ( int32_t )(__HAL_TIM_GET_COUNTER(&g_timx_encode_chy_handle) + g_encoder_t.timx_encode_count * 65536);
}



/**
 * @brief       ��ʱ�������жϻص����������жϻص���������TIM3��TIM6����;TIM3��¼���������
 * 				TIM6��ʱ����PID
 * @param        htim:��ʱ�����ָ��
 * @note        �˺����ᱻ��ʱ���жϺ�����ͬ���õ�
 * @retval      ��
 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t val=0;
    static bool flag_match_debug =false;
    if(htim->Instance == GTIM_TIMX_ENCODER)
    {
    	g_encoder_t.timx_encode_count += __HAL_TIM_IS_TIM_COUNTING_DOWN(&g_timx_encode_chy_handle)?
    									 -1:1;
//        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&g_timx_encode_chy_handle))
//        {
//        	g_encoder_t.timx_encode_count--;
//        }
//        else
//        {
//        	g_encoder_t.timx_encode_count++;
//        }
    }
    else if(htim->Instance == BTIM_TIMX_INT)
    {
    	g_encoder_t.encode_now = gtim_get_encode();        /* ��ȡ������ֵ�����ڼ����ٶ� */
        if(val == SMAPLSE_PID_PERIOD)
        {
        	g_encoder_t.g_speed = g_encoder_t.encode_now-g_encoder_t.encode_old;	//�������ֵ�仯��

        	g_encoder_t.encode_old=g_encoder_t.encode_now;				//ˢ�±���ֵ��¼

            if(g_encoder_t.g_run_flag)/* ������� */
            {
                g_step_motor.location=gtim_get_encode();
                //increment_pid_ctrl�ķ���ֵ�������ǰһ�����Ŀ��ֵ�ı仯��������0��ʾ��Ҫ���ӣ�С��0��ʾ��Ҫ����
                g_encoder_t.motor_pwm = increment_pid_ctrl(&g_location_pid, g_step_motor.location);


//            	printf("increment_pid_ctrl return value %d @setpoint value %d\r\n",
//            			g_encoder_t.motor_pwm,
//						g_location_pid.SetPoint);


                if( 0!= g_encoder_t.motor_pwm)
                {
                	flag_match_debug = false;
                	//printf("increment_pid_ctrl returned value %ld\r\n", g_encoder_t.motor_pwm);
                }
                else
                {
                	//������ֵƥ��ɹ������Թرն�ʱ����
                	if(flag_match_debug == false)
                	{
                		flag_match_debug = true;
                		//printf("increment_pid_ctrl return 0!\r\n");
                		stepper_stop(STEPPER_MOTOR_1);
                	}
                }
                (g_encoder_t.motor_pwm > 0) ? (g_step_motor.dir=CW) : (g_step_motor.dir=CCW);
                g_encoder_t.motor_pwm = abs(g_encoder_t.motor_pwm);
                 /*
                    �������20ms��ʱ���ڲ������ֻ�ܽ���150*1600/2400=100������  �����ٶ�
                    20ms������100�����壬ʵ�ʾ���1ms���5������Ҳ����1s���5000������
                    �����תһȦ��Ҫ1600������Ҳ���� 5000/1600 = 3.125תÿ��
                    �����rpm�Ļ���3.125*60 = 187.5rpm��תÿ���ӣ�
                */
                stepper_motion_ctrl(g_step_motor.dir, g_encoder_t.motor_pwm);
                //g_send_flag=1;
            }
            val = 0;
        }
        val ++;
    }
}



