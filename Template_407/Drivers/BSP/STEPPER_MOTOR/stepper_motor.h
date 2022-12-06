/**
 ****************************************************************************************************
 * @file        stepper_motor.h
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
#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include "sys.h"
#include <stdbool.h>

/******************************************************************************************/

#define SMAPLSE_PID_PERIOD	20	/*采样率 单位为ms*/
#define SECONDS_PER_MINUTE	60
#define MS_PER_SECOND		1000

#define PULSE_REV       1600                                /* 每圈脉冲数 */
#define TIM_FREQ        168000000U                          /* 定时器主频 */
#define T1_FREQ         (TIM_FREQ/168)                      /* 频率ft值 */

#define FREQ_UINT       (T1_FREQ/(SECOND/SAMPLING_PERIOD))  /* 对定时器的频率做单位换算,避免数值太大溢出 */
#define ENCODER_SPR     (float)(500*4)                      /* 编码器单圈线数*4倍频(根据编码器线数) */
#define MPR             50                                   /* 步进电机旋转一圈, 负载移动的距离(2*PI*r)*/
#define PPM             (ENCODER_SPR/MPR)                   /* 每mm内编码器的脉冲数;单位:Pules/mm */
#define MPP             ((float)(MPR)/ENCODER_SPR)          /* 编码器单步步进距离 */
#define FEEDBACK_CONST  (float)(PULSE_REV/ENCODER_SPR)      /* 编码器和步进电机驱动器的比值（表示每个编码器输出线数对应的步进电机脉冲数） */




//电机参数结构体
typedef struct
{
    uint8_t state;      /* 电机状态 */
    volatile uint8_t dir;        /* 电机方向 */
    float speed;        /* 电机实际速度 */
    float setspeed;     /* 电机实际速度 */
    int32_t location;   /* 电机位置 */
    __IO bool setPoint_flag;	/*用于指示PID设置点是否变化*/
} Motor_TypeDef;


/******************************************************************************************/
/* 步进电机引脚定义*/

#define STEPPER_MOTOR_1       1              /* 步进电机接口序号 */
#define STEPPER_MOTOR_2       2
#define STEPPER_MOTOR_3       3
#define STEPPER_MOTOR_4       4
/*     步进电机方向引脚定义     */

#define STEPPER_DIR1_GPIO_PIN                  GPIO_PIN_14
#define STEPPER_DIR1_GPIO_PORT                 GPIOF
#define STEPPER_DIR1_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF口时钟使能 */

#define STEPPER_DIR2_GPIO_PIN                  GPIO_PIN_12
#define STEPPER_DIR2_GPIO_PORT                 GPIOF
#define STEPPER_DIR2_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF口时钟使能 */

#define STEPPER_DIR3_GPIO_PIN                  GPIO_PIN_2
#define STEPPER_DIR3_GPIO_PORT                 GPIOB
#define STEPPER_DIR3_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB口时钟使能 */

#define STEPPER_DIR4_GPIO_PIN                  GPIO_PIN_2
#define STEPPER_DIR4_GPIO_PORT                 GPIOH
#define STEPPER_DIR4_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)    /* PH口时钟使能 */

/*     步进电机脱机引脚定义     */

#define STEPPER_EN1_GPIO_PIN                   GPIO_PIN_15
#define STEPPER_EN1_GPIO_PORT                  GPIOF
#define STEPPER_EN1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF口时钟使能 */

#define STEPPER_EN2_GPIO_PIN                   GPIO_PIN_13
#define STEPPER_EN2_GPIO_PORT                  GPIOF
#define STEPPER_EN2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF口时钟使能 */

#define STEPPER_EN3_GPIO_PIN                   GPIO_PIN_11
#define STEPPER_EN3_GPIO_PORT                  GPIOF
#define STEPPER_EN3_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF口时钟使能 */
 
#define STEPPER_EN4_GPIO_PIN                   GPIO_PIN_3
#define STEPPER_EN4_GPIO_PORT                  GPIOH
#define STEPPER_EN4_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)    /* PH口时钟使能 */

/*----------------------- 方向引脚控制 -----------------------------------*/
/* 由于我们使用的是共阳极解法,并且硬件对电平做了取反操作，所以当 x = 1有效，x = 0时无效*/
#define ST1_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

#define ST2_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

#define ST3_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR3_GPIO_PORT, STEPPER_DIR3_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR3_GPIO_PORT, STEPPER_DIR3_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

#define ST4_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR4_GPIO_PORT, STEPPER_DIR4_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR4_GPIO_PORT, STEPPER_DIR4_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

/*----------------------- 脱机引脚控制 -----------------------------------*/
/* 由于我们使用的是共阳极解法,并且硬件对电平做了取反操作，所以当 x = 1有效，x = 0时无效*/
#define ST1_EN(x)      do{ x ? \
                          HAL_GPIO_WritePin(STEPPER_EN1_GPIO_PORT, STEPPER_EN1_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(STEPPER_EN1_GPIO_PORT, STEPPER_EN1_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)    

#define ST2_EN(x)      do{ x ? \
                          HAL_GPIO_WritePin(STEPPER_EN2_GPIO_PORT, STEPPER_EN2_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(STEPPER_EN2_GPIO_PORT, STEPPER_EN2_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)    

#define ST3_EN(x)      do{ x ? \
                          HAL_GPIO_WritePin(STEPPER_EN3_GPIO_PORT, STEPPER_EN3_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(STEPPER_EN3_GPIO_PORT, STEPPER_EN3_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)    

#define ST4_EN(x)      do{ x ? \
                          HAL_GPIO_WritePin(STEPPER_EN4_GPIO_PORT, STEPPER_EN4_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(STEPPER_EN4_GPIO_PORT, STEPPER_EN4_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)                            
                        
enum dir
{
    CCW = 0,                                                /* 逆时针旋转 */
    CW ,                                                    /* 顺时针旋转 */
};


#define AXIS_X_DEFAULT_DIRECTION	CW;
#define AXIS_Y_DEFAULT_DIRECTION	CW

extern Motor_TypeDef  g_step_motor;                         /* 电机参数变量 */
/******************************************************************************************/
/* 外部接口函数*/
void stepper_init(uint16_t arr, uint16_t psc);              	/* 步进电机接口初始化 */
void stepper_star(uint8_t motor_num);                       	/* 开启步进电机 */
void stepper_stop(uint8_t motor_num);                       	/* 关闭步进电机 */
void stepper_motion_ctrl(uint8_t dir, uint16_t location_m); 	/* 步进电机位置运动控制函数 */
#endif
