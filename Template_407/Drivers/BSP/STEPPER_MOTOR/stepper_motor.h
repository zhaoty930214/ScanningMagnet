/**
 ****************************************************************************************************
 * @file        stepper_motor.h
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
#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include "sys.h"
#include <stdbool.h>

/******************************************************************************************/

#define SMAPLSE_PID_PERIOD	20	/*������ ��λΪms*/
#define SECONDS_PER_MINUTE	60
#define MS_PER_SECOND		1000

#define PULSE_REV       1600                                /* ÿȦ������ */
#define TIM_FREQ        168000000U                          /* ��ʱ����Ƶ */
#define T1_FREQ         (TIM_FREQ/168)                      /* Ƶ��ftֵ */

#define FREQ_UINT       (T1_FREQ/(SECOND/SAMPLING_PERIOD))  /* �Զ�ʱ����Ƶ������λ����,������ֵ̫����� */
#define ENCODER_SPR     (float)(500*4)                      /* ��������Ȧ����*4��Ƶ(���ݱ���������) */
#define MPR             50                                   /* ���������תһȦ, �����ƶ��ľ���(2*PI*r)*/
#define PPM             (ENCODER_SPR/MPR)                   /* ÿmm�ڱ�������������;��λ:Pules/mm */
#define MPP             ((float)(MPR)/ENCODER_SPR)          /* ������������������ */
#define FEEDBACK_CONST  (float)(PULSE_REV/ENCODER_SPR)      /* �������Ͳ�������������ı�ֵ����ʾÿ�����������������Ӧ�Ĳ�������������� */




//��������ṹ��
typedef struct
{
    uint8_t state;      /* ���״̬ */
    volatile uint8_t dir;        /* ������� */
    float speed;        /* ���ʵ���ٶ� */
    float setspeed;     /* ���ʵ���ٶ� */
    int32_t location;   /* ���λ�� */
    __IO bool setPoint_flag;	/*����ָʾPID���õ��Ƿ�仯*/
} Motor_TypeDef;


/******************************************************************************************/
/* ����������Ŷ���*/

#define STEPPER_MOTOR_1       1              /* ��������ӿ���� */
#define STEPPER_MOTOR_2       2
#define STEPPER_MOTOR_3       3
#define STEPPER_MOTOR_4       4
/*     ��������������Ŷ���     */

#define STEPPER_DIR1_GPIO_PIN                  GPIO_PIN_14
#define STEPPER_DIR1_GPIO_PORT                 GPIOF
#define STEPPER_DIR1_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF��ʱ��ʹ�� */

#define STEPPER_DIR2_GPIO_PIN                  GPIO_PIN_12
#define STEPPER_DIR2_GPIO_PORT                 GPIOF
#define STEPPER_DIR2_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF��ʱ��ʹ�� */

#define STEPPER_DIR3_GPIO_PIN                  GPIO_PIN_2
#define STEPPER_DIR3_GPIO_PORT                 GPIOB
#define STEPPER_DIR3_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB��ʱ��ʹ�� */

#define STEPPER_DIR4_GPIO_PIN                  GPIO_PIN_2
#define STEPPER_DIR4_GPIO_PORT                 GPIOH
#define STEPPER_DIR4_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)    /* PH��ʱ��ʹ�� */

/*     ��������ѻ����Ŷ���     */

#define STEPPER_EN1_GPIO_PIN                   GPIO_PIN_15
#define STEPPER_EN1_GPIO_PORT                  GPIOF
#define STEPPER_EN1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF��ʱ��ʹ�� */

#define STEPPER_EN2_GPIO_PIN                   GPIO_PIN_13
#define STEPPER_EN2_GPIO_PORT                  GPIOF
#define STEPPER_EN2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF��ʱ��ʹ�� */

#define STEPPER_EN3_GPIO_PIN                   GPIO_PIN_11
#define STEPPER_EN3_GPIO_PORT                  GPIOF
#define STEPPER_EN3_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)    /* PF��ʱ��ʹ�� */
 
#define STEPPER_EN4_GPIO_PIN                   GPIO_PIN_3
#define STEPPER_EN4_GPIO_PORT                  GPIOH
#define STEPPER_EN4_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)    /* PH��ʱ��ʹ�� */

/*----------------------- �������ſ��� -----------------------------------*/
/* ��������ʹ�õ��ǹ������ⷨ,����Ӳ���Ե�ƽ����ȡ�����������Ե� x = 1��Ч��x = 0ʱ��Ч*/
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

/*----------------------- �ѻ����ſ��� -----------------------------------*/
/* ��������ʹ�õ��ǹ������ⷨ,����Ӳ���Ե�ƽ����ȡ�����������Ե� x = 1��Ч��x = 0ʱ��Ч*/
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
    CCW = 0,                                                /* ��ʱ����ת */
    CW ,                                                    /* ˳ʱ����ת */
};


#define AXIS_X_DEFAULT_DIRECTION	CW;
#define AXIS_Y_DEFAULT_DIRECTION	CW

extern Motor_TypeDef  g_step_motor;                         /* ����������� */
/******************************************************************************************/
/* �ⲿ�ӿں���*/
void stepper_init(uint16_t arr, uint16_t psc);              	/* ��������ӿڳ�ʼ�� */
void stepper_star(uint8_t motor_num);                       	/* ����������� */
void stepper_stop(uint8_t motor_num);                       	/* �رղ������ */
void stepper_motion_ctrl(uint8_t dir, uint16_t location_m); 	/* �������λ���˶����ƺ��� */
#endif
