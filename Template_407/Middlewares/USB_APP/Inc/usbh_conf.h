/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Inc/usbh_conf.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    29-December-2017
  * @brief   General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __USBH_CONF__H__
#define __USBH_CONF__H__
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f4xx_hal_hcd.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"


#define HAL_HCD_MODULE_ENABLED                      /* 使能HCD模块,必须设置,否则将不会编译pcd相关代码 */

/*----------   -----------*/
#define USBH_MAX_NUM_ENDPOINTS      2U
 
/*----------   -----------*/
#define USBH_MAX_NUM_INTERFACES      2U
 
/*----------   -----------*/
#define USBH_MAX_NUM_CONFIGURATION      1U

/*----------   -----------*/
#define USBH_KEEP_CFG_DESCRIPTOR      1U
 
/*----------   -----------*/
#define USBH_MAX_NUM_SUPPORTED_CLASS      1U
 
/*----------   -----------*/
#define USBH_MAX_SIZE_CONFIGURATION      512U
 
/*----------   -----------*/
#define USBH_MAX_DATA_BUFFER      512U
 
/*----------   -----------*/
#define USBH_DEBUG_LEVEL      0U
 
/*----------   -----------*/
#define USBH_USE_OS      0U
 
 
void USB_Init(void);

/****************************************/
/* #define for FS and HS identification */
#define HOST_HS 		0
#define HOST_FS 		1

#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define USBH_PROCESS_PRIO          osPriorityNormal
  #define USBH_PROCESS_STACK_SIZE    ((uint16_t)0)
#endif /* (USBH_USE_OS == 1) */

/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_Macros USBH_CONF_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* Memory management macros */

/** Alias for memory allocation. */
#define USBH_malloc(x)         malloc(x)

/** Alias for memory release. */
#define USBH_free(x)           free(x)

/** Alias for memory set. */
#define USBH_memset         memset

/** Alias for memory copy. */
#define USBH_memcpy         memcpy

/* DEBUG macros */

#if (USBH_DEBUG_LEVEL > 0U)
#define  USBH_UsrLog(...)   do { \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_UsrLog(...) do {} while (0)
#endif

#if (USBH_DEBUG_LEVEL > 1U)

#define  USBH_ErrLog(...) do { \
                            printf("ERROR: ") ; \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_ErrLog(...) do {} while (0)
#endif

#if (USBH_DEBUG_LEVEL > 2U)
#define  USBH_DbgLog(...)   do { \
                            printf("DEBUG : ") ; \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_DbgLog(...) do {} while (0)
#endif


#ifdef __cplusplus
}
#endif

#endif /* __USBH_CONF__H__ */
