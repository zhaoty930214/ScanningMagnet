#ifndef _PHOTOELECTRIC_SWITCH_H
#define _PHOTOELECTRIC_SWITCH_H

#include "stm32f4xx.h"
//#include "core_cm4.h"
//#include "stm32f4xx_hal.h"


/*************光电开关引脚定义********************/
#define OPTICAL_KEY_1_PORT	GPIOC
#define OPTICAL_KEY_2_PORT	GPIOF
#define OPTICAL_KEY_3_PORT	GPIOH

#define OPTICAL_KEY_1_PIN	GPIO_PIN_0
#define OPTICAL_KEY_2_PIN	GPIO_PIN_4
#define OPTICAL_KEY_3_PIN	GPIO_PIN_13


#define TEMPER_FLOW_INTERLOCK_PORT	GPIOB
#define TEMPER_FLOW_INTERLOCK_PIN	GPIO_PIN_1

#define THERMAL_COUPLING_INTERLOCK_PORT	GPIOF
#define THERMAL_COUPLING_INTERLOCK_PIN	GPIO_PIN_5
/*************光电开关引脚定义********************/


/******************************************************************************************/
/* 引脚 和 中断编号 & 中断服务函数 定义 */
/*3颗光电开关，2个外部中断(联锁)******************/
#define SWITCH1_INT_IRQn                   EXTI0_IRQn
#define SWITCH1_INT_IRQHandler             EXTI0_IRQHandler
#define SWITCH1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

#define SWITCH2_INT_IRQn                   EXTI4_IRQn
#define SWITCH2_INT_IRQHandler             EXTI4_IRQHandler
#define SWITCH2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

#define SWITCH3_INT_IRQn                   EXTI15_10_IRQn
#define SWITCH3_INT_IRQHandler             EXTI15_10_IRQHandler
#define SWITCH3_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

#define TEMPER_FLOW_IRQn		   EXTI1_IRQn
#define TEMPER_FLOW_IRQHandler			   EXTI1_IRQHandler
#define TEMPER_FLOW_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

#define THERMAL_COUPLING_IRQn	   		EXTI9_5_IRQn
#define THERMAL_COUPLING_IRQHandler				EXTI9_5_IRQHandler
#define THERMAL_COUPLING_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

/*3颗光电开关，2个外部中断(联锁)******************/


void init_photoelectric_switch(void);
void init_input_io(void);
#endif

