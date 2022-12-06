/**
 ****************************************************************************************************
 * @file        usbh_conf.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-26
 * @brief       usbh_conf ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ��www.yuanzige.com
 * ������̳��http://www.openedv.com/forum.php
 * ��˾��ַ��www.alientek.com
 * �����ַ��zhengdianyuanzi.tmall.com
 *
 * �޸�˵��
 * V1.0 20211126
 * ��һ�η���
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
 * @brief       USB OTG �жϷ�����
 *   @note      ��������USB�ж�
 * @param       ��
 * @retval      ��
 */
void OTG_FS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&g_hhcd_USB_OTG_FS);
}
/**
 * @brief       ��ʼ��HCD MSP
 *   @note      ����һ���ص�����, ��stm32f4xx_hal_hcd.c�������
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hcdHandle)
{
    GPIO_InitTypeDef gpio_init_struct= { 0 };

    if (hcdHandle->Instance == USB_OTG_FS)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();                           /* ʹ��GPIOAʱ�� */

        gpio_init_struct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ���� */
        gpio_init_struct.Pull = GPIO_NOPULL;                    /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
        gpio_init_struct.Alternate = GPIO_AF10_OTG_FS;          /* ����ΪOTG_FS */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);                /* ��ʼ��PA11��PA12���� */

        USBH_Delay(100);

        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();          /* ʹ��OTG FSʱ�� */

        HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 2);    /* ���ȼ�����Ϊ��ռ1,�����ȼ�2 */
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);            /* ʹ��OTG FS�ж� */
    }
}

/**
 * @brief       ȡ����ʼ��HCD MSP
 *   @note      ����һ���ص�����, ��stm32f4xx_hal_hcd.c�������
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
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
/* ������: USBH LL HCD �����Ļص�����(HCD->USB Host Library) */

/**
 * @brief       USBH SOF�ص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer(hhcd->pData);
}

/**
 * @brief       USBH ���ӳɹ��ص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("Connected \r\n");
    USBH_LL_Connect(hhcd->pData);
}

/**
 * @brief       USBH �Ͽ����ӻص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("Disconnected \r\n");
    USBH_LL_Disconnect(hhcd->pData);
}

/**
 * @brief       USBH �ӿ�ʹ�ܻص�����(V3.3.3 USB�������ص�����)
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("connect port open \r\n");
    USBH_LL_PortEnabled(hhcd->pData);
}

/**
 * @brief       USBH �ӿڹرջص�����(V3.3.3 USB�������ص�����)
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("connect port close \r\n");
    USBH_LL_PortDisabled(hhcd->pData);
}

/**
 * @brief       USBH ֪ͨURB�仯�ص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @param       chnum       : �˵���
 * @param       urb_state   : URB״̬
 * @retval      ��
 */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
    /* To be used with OS to sync URB state with the global state machine */
//    printf("USB check callback \r\n");
}

/******************************************************************************************/
/* ������: USBH LL HCD �����ӿں���(HCD->USB Host Library) */

/**
 * @brief       USBH �ײ��ʼ������
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
    /* Init USB_IP */
    if (phost->id == HOST_FS)
    {
        /* Link the driver to the stack. */
        g_hhcd_USB_OTG_FS.Instance = USB_OTG_FS;                /* ʹ��USB_OTG */
        g_hhcd_USB_OTG_FS.Init.Host_channels = 8;               /* ����ͨ����Ϊ8�� */
        g_hhcd_USB_OTG_FS.Init.speed = HCD_SPEED_FULL;          /* USBȫ�� 12M */
        g_hhcd_USB_OTG_FS.Init.dma_enable = DISABLE;            /* ��ʹ��DMA */
        g_hhcd_USB_OTG_FS.Init.phy_itface = HCD_PHY_EMBEDDED;   /* ʹ���ڲ�PHY */
        g_hhcd_USB_OTG_FS.Init.Sof_enable = DISABLE;            /* ��ֹSOF�ж� */
        g_hhcd_USB_OTG_FS.Init.vbus_sensing_enable = 0;         /* ��ʹ��VBUS��� */
        g_hhcd_USB_OTG_FS.Init.lpm_enable = 0;                  /* ��ʹ�����ӵ�Դ���� */
        g_hhcd_USB_OTG_FS.Init.low_power_enable = 0;            /* ��ʹ�ܵ͹���ģʽ */

        g_hhcd_USB_OTG_FS.pData = phost;    /* g_hhcd_USB_OTG_FS��pDataָ��phost */
        phost->pData = &g_hhcd_USB_OTG_FS;  /* phost��pDataָ��g_hhcd_USB_OTG_FS */

        
        if (HAL_HCD_Init(&g_hhcd_USB_OTG_FS) != HAL_OK)     /* ��ʼ��LL���� */
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
 * @brief       USBH �ײ�ȡ����ʼ��(�ָ�Ĭ�ϸ�λ״̬)����
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH �ײ�������ʼ����
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH �ײ�����ֹͣ����
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH ��ȡUSB�豸���ٶ�
 * @param       phost       : USBH���ָ��
 * @retval      USBH �豸�ٶ�
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
 * @brief       USBH ��λUSB HOST�˿�
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH ��ȡ���һ�δ���İ���С
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      ����С
 */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
 * @brief       USBH ��һ���ܵ�(ͨ��)
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @param       epnum       : �˵��
 * @param       dev_address : �豸��ַ
 * @param       speed       : �豸�ٶ�
 * @param       ep_type     : �˵�����
 * @param       mps         : �˵�������С
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH �ر�һ���ܵ�(ͨ��)
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH �ύһ���µ�URB
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @param       direction   : ��������
 * @param       ep_type     : �˵�����
 * @param       token       : �˵��־
 * @param       pbuff       : URB�������׵�ַ
 * @param       length      : URB���ݳ���
 * @param       do_ping     : ����do ping protocol,USB HS���õ�
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH ��ȡURB״̬
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      URB״̬
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
 * @brief       USBH ����VBUS״̬
 * @param       phost       : USBH���ָ��
 * @param       state       : vbus״̬. 0,����; 1,������
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
    if (phost->id == HOST_FS)
    {
        if (state == 0)
        {
            /* û���õ�VBUS��� */
        }
        else
        {
            /* û���õ�VBUS��� */
        }
    }

    USBH_Delay(200);
    return USBH_OK;
}

/**
 * @brief       USBH ���ùܵ��ķ�ת
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @param       toggle      : ��ת״̬
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
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
 * @brief       USBH ��ȡ�ܵ���ת״̬
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      ��ת״̬
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
 * @brief       USBH ��ʱ����(��msΪ��λ)
 * @param       Delay       : ��ʱ��ms��
 * @retval      ��ת״̬
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

