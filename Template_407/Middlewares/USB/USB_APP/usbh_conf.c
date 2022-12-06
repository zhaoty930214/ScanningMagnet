/**
 ****************************************************************************************************
 * @file        usbh_conf.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-26
 * @brief       usbh_conf 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频：www.yuanzige.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 *
 * 修改说明
 * V1.0 20211126
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "usbh_conf.h"
#include "usbh_core.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


HCD_HandleTypeDef g_hhcd_USB_OTG_FS;

USBH_StatusTypeDef USBH_Get_USB_Status(HAL_StatusTypeDef hal_status);

/**
 * @brief       USB OTG 中断服务函数
 *   @note      处理所有USB中断
 * @param       无
 * @retval      无
 */
void OTG_FS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&g_hhcd_USB_OTG_FS);
}
/**
 * @brief       初始化HCD MSP
 *   @note      这是一个回调函数, 在stm32f4xx_hal_hcd.c里面调用
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hcdHandle)
{
    GPIO_InitTypeDef gpio_init_struct= { 0 };

    if (hcdHandle->Instance == USB_OTG_FS)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();                           /* 使能GPIOA时钟 */

        gpio_init_struct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用 */
        gpio_init_struct.Pull = GPIO_NOPULL;                    /* 浮空 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 高速 */
        gpio_init_struct.Alternate = GPIO_AF10_OTG_FS;          /* 复用为OTG_FS */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);                /* 初始化PA11和PA12引脚 */

        USBH_Delay(100);

        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();          /* 使能OTG FS时钟 */

        HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 2);    /* 优先级设置为抢占1,子优先级2 */
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);            /* 使能OTG FS中断 */
    }
}

/**
 * @brief       取消初始化HCD MSP
 *   @note      这是一个回调函数, 在stm32f4xx_hal_hcd.c里面调用
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hcdHandle)
{
    if (hcdHandle->Instance == USB_OTG_FS)
    {
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

        /**USB_OTG_FS GPIO Configuration
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
    }
}

/******************************************************************************************/
/* 以下是: USBH LL HCD 驱动的回调函数(HCD->USB Host Library) */

/**
 * @brief       USBH SOF回调函数
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer(hhcd->pData);
}

/**
 * @brief       USBH 连接成功回调函数
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("Connected \r\n");
    USBH_LL_Connect(hhcd->pData);
}

/**
 * @brief       USBH 断开连接回调函数
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("Disconnected \r\n");
    USBH_LL_Disconnect(hhcd->pData);
}

/**
 * @brief       USBH 接口使能回调函数(V3.3.3 USB库新增回调函数)
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("connect port open \r\n");
    USBH_LL_PortEnabled(hhcd->pData);
}

/**
 * @brief       USBH 接口关闭回调函数(V3.3.3 USB库新增回调函数)
 * @param       hhcd        : HCD结构体指针
 * @retval      无
 */
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("connect port close \r\n");
    USBH_LL_PortDisabled(hhcd->pData);
}

/**
 * @brief       USBH 通知URB变化回调函数
 * @param       hhcd        : HCD结构体指针
 * @param       chnum       : 端点编号
 * @param       urb_state   : URB状态
 * @retval      无
 */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
    /* To be used with OS to sync URB state with the global state machine */
//    printf("USB check callback \r\n");
}

/******************************************************************************************/
/* 以下是: USBH LL HCD 驱动接口函数(HCD->USB Host Library) */

/**
 * @brief       USBH 底层初始化函数
 * @param       phost       : USBH句柄指针
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
    /* Init USB_IP */
    if (phost->id == HOST_FS)
    {
        /* Link the driver to the stack. */
        g_hhcd_USB_OTG_FS.Instance = USB_OTG_FS;                /* 使用USB_OTG */
        g_hhcd_USB_OTG_FS.Init.Host_channels = 8;               /* 主机通道数为8个 */
        g_hhcd_USB_OTG_FS.Init.speed = HCD_SPEED_FULL;          /* USB全速 12M */
        g_hhcd_USB_OTG_FS.Init.dma_enable = DISABLE;            /* 不使用DMA */
        g_hhcd_USB_OTG_FS.Init.phy_itface = HCD_PHY_EMBEDDED;   /* 使能内部PHY */
        g_hhcd_USB_OTG_FS.Init.Sof_enable = DISABLE;            /* 禁止SOF中断 */
        g_hhcd_USB_OTG_FS.Init.vbus_sensing_enable = 0;         /* 不使能VBUS检测 */
        g_hhcd_USB_OTG_FS.Init.lpm_enable = 0;                  /* 不使能连接电源管理 */
        g_hhcd_USB_OTG_FS.Init.low_power_enable = 0;            /* 不使能低功耗模式 */

        g_hhcd_USB_OTG_FS.pData = phost;    /* g_hhcd_USB_OTG_FS的pData指向phost */
        phost->pData = &g_hhcd_USB_OTG_FS;  /* phost的pData指向g_hhcd_USB_OTG_FS */

        
        if (HAL_HCD_Init(&g_hhcd_USB_OTG_FS) != HAL_OK)     /* 初始化LL驱动 */
        {
            printf("USB Init ERROR \r\n");
        }
        else
        {
            printf("USB Init OK \r\n");
        }
        USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&g_hhcd_USB_OTG_FS));
    }

    return USBH_OK;
}

/**
 * @brief       USBH 底层取消初始化(恢复默认复位状态)函数
 * @param       phost       : USBH句柄指针
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_DeInit(phost->pData);

    usb_status = USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 底层驱动开始工作
 * @param       phost       : USBH句柄指针
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_Start(phost->pData);

    usb_status = USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 底层驱动停止工作
 * @param       phost       : USBH句柄指针
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_Stop(phost->pData);

    usb_status = USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 获取USB设备的速度
 * @param       phost       : USBH句柄指针
 * @retval      USBH 设备速度
 */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

    switch (HAL_HCD_GetCurrentSpeed(phost->pData))
    {
        case 0 :
            speed = USBH_SPEED_HIGH;
            printf("USB Host [HS]\r\n");
            break;

        case 1 :
            speed = USBH_SPEED_FULL;
            printf("USB Host [FS]\r\n");
            break;

        case 2 :
            speed = USBH_SPEED_LOW;
            printf("USB Host [LS]\r\n");
            break;

        default:
            printf("USB Host [FS?]\r\n");
            speed = USBH_SPEED_FULL;
            break;
    }

    return  speed;
}

/**
 * @brief       USBH 复位USB HOST端口
 * @param       phost       : USBH句柄指针
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
    printf("USB reset port \r\n");
    delay_ms(100);

    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_ResetPort(phost->pData);

    usb_status = USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 获取最近一次传输的包大小
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @retval      包大小
 */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
 * @brief       USBH 打开一个管道(通道)
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @param       epnum       : 端点号
 * @param       dev_address : 设备地址
 * @param       speed       : 设备速度
 * @param       ep_type     : 端点类型
 * @param       mps         : 端点最大包大小
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe_num, uint8_t epnum,
                                    uint8_t dev_address, uint8_t speed, uint8_t ep_type, uint16_t mps)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_HC_Init(phost->pData, pipe_num, epnum,
                                 dev_address, speed, ep_type, mps);

    usb_status = USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 关闭一个管道(通道)
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_HC_Halt(phost->pData, pipe);

    usb_status = USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 提交一个新的URB
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @param       direction   : 方向设置
 * @param       ep_type     : 端点类型
 * @param       token       : 端点标志
 * @param       pbuff       : URB数据区首地址
 * @param       length      : URB数据长度
 * @param       do_ping     : 激活do ping protocol,USB HS才用到
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t direction,
                                     uint8_t ep_type, uint8_t token, uint8_t *pbuff, uint16_t length,
                                     uint8_t do_ping)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBH_StatusTypeDef usb_status = USBH_OK;

    hal_status = HAL_HCD_HC_SubmitRequest(phost->pData, pipe, direction,
                                          ep_type, token, pbuff, length,
                                          do_ping);
    usb_status =  USBH_Get_USB_Status(hal_status);

    return usb_status;
}

/**
 * @brief       USBH 获取URB状态
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @retval      URB状态
  *     @arg URB_IDLE
  *     @arg URB_DONE
  *     @arg URB_NOTREADY
  *     @arg URB_NYET
  *     @arg URB_ERROR
  *     @arg URB_STALL
  */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState (phost->pData, pipe);
}

/**
 * @brief       USBH 控制VBUS状态
 * @param       phost       : USBH句柄指针
 * @param       state       : vbus状态. 0,激活; 1,不激活
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
    if (phost->id == HOST_FS)
    {
        if (state == 0)
        {
            /* 没有用到VBUS检测 */
        }
        else
        {
            /* 没有用到VBUS检测 */
        }
    }

    USBH_Delay(200);
    return USBH_OK;
}

/**
 * @brief       USBH 设置管道的翻转
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @param       toggle      : 翻转状态
 * @retval      USB状态
 *   @arg       USBH_OK(0)   , 正常;
 *   @arg       USBH_BUSY(1) , 忙;
 *   @arg       其他         , 失败;
 */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
    HCD_HandleTypeDef *pHandle;
    pHandle = phost->pData;

    if (pHandle->hc[pipe].ep_is_in)
    {
        pHandle->hc[pipe].toggle_in = toggle;
    }
    else
    {
        pHandle->hc[pipe].toggle_out = toggle;
    }

    return USBH_OK;
}

/**
 * @brief       USBH 获取管道翻转状态
 * @param       phost       : USBH句柄指针
 * @param       pipe        : 管道编号
 * @retval      翻转状态
 */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    uint8_t toggle = 0;
    HCD_HandleTypeDef *pHandle;
    pHandle = phost->pData;

    if (pHandle->hc[pipe].ep_is_in)
    {
        toggle = pHandle->hc[pipe].toggle_in;
    }
    else
    {
        toggle = pHandle->hc[pipe].toggle_out;
    }

    return toggle;
}

/**
 * @brief       USBH 延时函数(以ms为单位)
 * @param       Delay       : 延时的ms数
 * @retval      翻转状态
 */
void USBH_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}

/**
  * @brief  Retuns the USB status depending on the HAL status:
  * @param  hal_status: HAL status
  * @retval USB status
  */
USBH_StatusTypeDef USBH_Get_USB_Status(HAL_StatusTypeDef hal_status)
{
    USBH_StatusTypeDef usb_status = USBH_OK;

    switch (hal_status)
    {
        case HAL_OK :
            usb_status = USBH_OK;
            break;

        case HAL_ERROR :
            usb_status = USBH_FAIL;
            break;

        case HAL_BUSY :
            usb_status = USBH_BUSY;
            break;

        case HAL_TIMEOUT :
            usb_status = USBH_FAIL;
            break;

        default :
            usb_status = USBH_FAIL;
            break;
    }

    return usb_status;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

