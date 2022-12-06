/**
 ****************************************************************************************************
 * @file        usbh_diskio.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-26
 * @brief       usbh diskio ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� F407���������
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

#include "usbh_diskio.h"
#include "ff.h"
#include "usart.h"


extern USBH_HandleTypeDef g_hUSBHost;

/**
 * @brief       ��ʼ�� USBH
 * @param       ��
 * @retval      ��
 */
DSTATUS USBH_initialize(void)
{
    return RES_OK;
}

/**
 * @brief       ��ȡU��״̬
 * @param       ��
 * @retval      ��
 */
DSTATUS USBH_status(void)
{
    DRESULT res = RES_ERROR;
    MSC_HandleTypeDef *MSC_Handle = g_hUSBHost.pActiveClass->pData;

    if (USBH_MSC_UnitIsReady(&g_hUSBHost, MSC_Handle->current_lun))
    {
        printf("U��״̬��ѯ�ɹ�\r\n");
        res = RES_OK;
    }
    else
    {
        printf("U��״̬��ѯʧ��\r\n");
        res = RES_ERROR;
    }

    return res;
}

/**
 * @brief       U�̶���������
 * @param       buff   : ���ݻ����׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫ��ȡ��������
 * @retval      ִ�н��(���DRESULT����)
 */
DRESULT USBH_read(BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    MSC_HandleTypeDef *MSC_Handle = g_hUSBHost.pActiveClass->pData;
    
    if (USBH_MSC_Read(&g_hUSBHost, MSC_Handle->current_lun, sector, buff, count) == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        printf("U�̶�ȡʧ��\r\n");
        USBH_MSC_GetLUNInfo(&g_hUSBHost, MSC_Handle->current_lun, &info);
        switch (info.sense.asc)
        {
            case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case SCSI_ASC_MEDIUM_NOT_PRESENT:
            case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                USBH_ErrLog("USB Disk is not ready!");
                res = RES_NOTRDY;
                break;
            default:
                res = RES_ERROR;
                break;
        }
    }
    return res;
}


/**
 * @brief       U��д��������
 * @param       buff   : ���ݻ����׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫд���������
 * @retval      ִ�н��(���DRESULT����)
 */
DRESULT USBH_write(const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    MSC_HandleTypeDef *MSC_Handle = g_hUSBHost.pActiveClass->pData;
    if (USBH_MSC_Write(&g_hUSBHost, MSC_Handle->current_lun, sector, (BYTE *)buff, count) == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        printf("U��д��ʧ��\r\n");
        USBH_MSC_GetLUNInfo(&g_hUSBHost, MSC_Handle->current_lun, &info);
        switch (info.sense.asc)
        {
            case SCSI_ASC_WRITE_PROTECTED:
                USBH_ErrLog("USB Disk is Write protected!");
                res = RES_WRPRT;
                break;
            case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case SCSI_ASC_MEDIUM_NOT_PRESENT:
            case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                USBH_ErrLog("USB Disk is not ready!");
                res = RES_NOTRDY;
                break;
            default:
                res = RES_ERROR;
                break;
        }
    }
    return res;
}

/**
 * @brief       U��IO���Ʋ���
 * @param       cmd    : ��������
 * @param       buff   : ��������
 * @retval      ִ�н��(���DRESULT����)
 */
DRESULT USBH_ioctl(BYTE cmd,void *buff)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    MSC_HandleTypeDef *MSC_Handle = g_hUSBHost.pActiveClass->pData;
    switch(cmd)
    {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT : /* ��ȡ�������� */
            if (USBH_MSC_GetLUNInfo(&g_hUSBHost, MSC_Handle->current_lun, &info) == USBH_OK)
            {
                *(DWORD*)buff = info.capacity.block_nbr;
                res = RES_OK;
                printf("��������:%d\r\n", info.capacity.block_nbr);
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        case GET_SECTOR_SIZE : /* ��ȡ������С */
            if (USBH_MSC_GetLUNInfo(&g_hUSBHost, MSC_Handle->current_lun, &info) == USBH_OK)
            {
                *(DWORD*)buff = info.capacity.block_size;
                res = RES_OK;
                printf("������С:%d\r\n", info.capacity.block_size);
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        case GET_BLOCK_SIZE :   /* ��ȡһ���������������Ĵ�С */

            if(USBH_MSC_GetLUNInfo(&g_hUSBHost, MSC_Handle->current_lun, &info) == USBH_OK)
            {
                *(DWORD*)buff = info.capacity.block_size/USB_DEFAULT_BLOCK_SIZE;
                printf("ÿ������������:%d\r\n", info.capacity.block_size/USB_DEFAULT_BLOCK_SIZE);
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;
        default:
            res = RES_PARERR;
            break;
    }
    return res;
}
