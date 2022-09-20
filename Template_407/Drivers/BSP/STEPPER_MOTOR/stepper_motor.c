/**
 ****************************************************************************************************
 * @file        stepper_motor.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       步进电机 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
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
 * @brief       初始化步进电机相关IO口, 并使能时钟
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void stepper_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;

    STEPPER_DIR1_GPIO_CLK_ENABLE();                                 /* DIR1时钟使能 */
    STEPPER_DIR2_GPIO_CLK_ENABLE();                                 /* DIR2时钟使能 */
    STEPPER_DIR3_GPIO_CLK_ENABLE();                                 /* DIR3时钟使能 */
    STEPPER_DIR4_GPIO_CLK_ENABLE();                                 /* DIR4时钟使能 */
            
    STEPPER_EN1_GPIO_CLK_ENABLE();                                  /* EN1时钟使能 */
    STEPPER_EN2_GPIO_CLK_ENABLE();                                  /* EN2时钟使能 */
    STEPPER_EN3_GPIO_CLK_ENABLE();                                  /* EN3时钟使能 */
    STEPPER_EN4_GPIO_CLK_ENABLE();                                  /* EN4时钟使能 */
    

    gpio_init_struct.Pin = STEPPER_DIR1_GPIO_PIN;                   /* DIR1引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;                    /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                          /* 下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;                   /* 低速 */
    HAL_GPIO_Init(STEPPER_DIR1_GPIO_PORT, &gpio_init_struct);       /* 初始化DIR1引脚 */

    gpio_init_struct.Pin = STEPPER_DIR2_GPIO_PIN;                   /* DIR2引脚 */
    HAL_GPIO_Init(STEPPER_DIR2_GPIO_PORT, &gpio_init_struct);       /* 初始化DIR2引脚 */

    gpio_init_struct.Pin = STEPPER_DIR3_GPIO_PIN;                   /* DIR3引脚 */
    HAL_GPIO_Init(STEPPER_DIR3_GPIO_PORT, &gpio_init_struct);       /* 初始化DIR3引脚 */

    gpio_init_struct.Pin = STEPPER_DIR4_GPIO_PIN;                   /* DIR4引脚 */
    HAL_GPIO_Init(STEPPER_DIR4_GPIO_PORT, &gpio_init_struct);       /* 初始化DIR4引脚 */
    
    /*   脱机引脚初始化   */
    
    gpio_init_struct.Pin = STEPPER_EN1_GPIO_PIN;                    /* EN1引脚 */
    HAL_GPIO_Init(STEPPER_EN1_GPIO_PORT, &gpio_init_struct);        /* 初始化EN1引脚 */
    
    gpio_init_struct.Pin = STEPPER_EN2_GPIO_PIN;                    /* EN2引脚 */
    HAL_GPIO_Init(STEPPER_EN2_GPIO_PORT, &gpio_init_struct);        /* 初始化EN2引脚 */
    
    gpio_init_struct.Pin = STEPPER_EN3_GPIO_PIN;                    /* EN3引脚 */
    HAL_GPIO_Init(STEPPER_EN3_GPIO_PORT, &gpio_init_struct);        /* 初始化EN3引脚 */
    
    gpio_init_struct.Pin = STEPPER_EN4_GPIO_PIN;                    /* EN4引脚 */
    HAL_GPIO_Init(STEPPER_EN4_GPIO_PORT, &gpio_init_struct);        /* 初始化EN4引脚 */
    
    atim_timx_oc_chy_init(arr, psc);                                /* 初始化PUL引脚，以及脉冲模式等 */
}


/**
 * @brief       开启步进电机
 * @param       motor_num: 步进电机接口序号
 * @retval      无
 */
void stepper_star(uint8_t motor_num)
{
    switch(motor_num)
    {
        /* 开启对应PWM通道 */
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
 * @brief       关闭步进电机
 * @param       motor_num: 步进电机接口序号
 * @retval      无
 */
void stepper_stop(uint8_t motor_num)
{
    switch(motor_num)
    {
        case STEPPER_MOTOR_1 :
        {
            /* 关闭对应PWM通道 */
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

/********************************** 步进电机控制代码 *****************************************/
/*全局变量*/

Motor_TypeDef  g_step_motor;                                    /* 电机参数变量 */
volatile uint16_t g_step_angle = 65535;                         /* 设置的步进角度 */

/**
 * @brief: 步进电机位置运动控制,注意此函数只在定时器中断服务函数中被调用
 * @param: dir:步进电机运动方向 0:反转 1正转
           location_m: PID输出的期望值（该期望值为编码器数值）
 * @retval 无
 */
void stepper_motion_ctrl(uint8_t dir, uint16_t location_m)      /* location_m如果这个值设置100，那就是20ms的采样频率内要输出100个脉冲信号 */
{
    float step_delay = 0.0;                                     /* 步进延时 */
    static int debug_out = 0;
    int queue_item;
    if(location_m == 0)
    {
        HAL_TIM_OC_Stop_IT(&g_atimx_handle, ATIM_TIMX_PWM_CH1);  /* 当期望值与目标值一致时，代表已到指定位置，停止输出 */

        g_step_angle = 0;                                        /* 清空数据 */
        if(debug_out == 0)
        {
        	debug_out = 1;
            //printf("Motor has rotated to special location!\r\n");
            //printf("Current encoder value is %d\r\n", gtim_get_encode());
            //从中断中向队列发送消息,需要区分3个轴的闭环控制ID，这里暂时写固定0x11
            queue_item = 0x11;
            xQueueSendFromISR(Queue_MotorReady, &queue_item, false);
        }
    }
    else
    {
    	/*clear debug_out recortd */
    	debug_out = 0;

        if(dir == CW)                                           /* 设置旋转方向 */
        	ST1_DIR(CW);
        else
        	ST1_DIR(CCW);

        /* 经过PID计算得到的结果是编码器的输出期望值,
         * location_m * FEEDBACK_CONST = 编码器脉冲数值转换之后的步进电机脉冲数
         * step_dealy = 20ms/location_m * FEEDBACK_CONST， 则等于步进电机每个脉冲周期所对应的时间，单位为ms
          将其转换为步进电机所需的脉冲数（ 编码器期望值 * （步进电机一圈所需脉冲数 / 编码器一圈计数值））*/
        step_delay = (float)(SMAPLSE_PID_PERIOD /(location_m * FEEDBACK_CONST));   /* 单个脉冲的时间宽度，单位： ms */

        /* ms转成s step_delay = step_delay/1000 */
        /*
           T : 单个脉冲所需的时间
           c : 需要求解的比较值
           f : 定时器计数频率
           T  = c *(1/f)
           step_delay/1000 = c*1/1000000 (单位是s)
           c = step_delay*1000   求出整一个脉冲的计数值，需要除以2 因为一个完整的脉冲需要翻转2次
           所要的计数值：c/2 = Sstep_delay*1000/2 = step_delay*500
        */
        /*模拟编码器分支*/
        g_step_angle = (uint16_t)(step_delay * 500);            /* 算出来的结果是周期,这里除以2,半周期翻转一次 */

        //g_step_angle = location_m * FEEDBACK_CONST;

        //printf("We want another cnt %d\r\n", g_step_angle);
        HAL_TIM_OC_Start_IT(&g_atimx_handle,ATIM_TIMX_PWM_CH1);
        //HAL_TIM_OC_Start_IT(&g_atimx_handle,ATIM_TIMX_PWM_CH2);
    }
}

