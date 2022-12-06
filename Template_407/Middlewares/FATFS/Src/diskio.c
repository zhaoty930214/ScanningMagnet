/**
 ****************************************************************************************************
 * @file        diskio.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2022-01-14
 * @brief       FATFS�ײ�(diskio) ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� STM32������
 * ������Ƶ��www.yuanzige.com
 * ������̳��http://www.openedv.com/forum.php
 * ��˾��ַ��www.alientek.com
 * �����ַ��zhengdianyuanzi.tmall.com
 *
 * �޸�˵��
 * V1.0 20220114
 * ��һ�η���
 * V1.1 20220120
 * ������U�̵�֧��
 *
 ****************************************************************************************************
 */

#include "malloc.h"
#include "usart.h"
#include "diskio.h"
#include "usbh_diskio.h"


#define USB_DISK    0       /* U��,���Ϊ0 */


/**
 * @brief       ��ô���״̬
 * @param       pdrv : ���̱��0~9
 * @retval      ��
 */
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    return RES_OK;
}

/**
 * @brief       ��ʼ������
 * @param       pdrv : ���̱��0~9
 * @retval      ��
 */
DSTATUS disk_initialize (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    uint8_t res = 0;

    switch (pdrv)
    {

        case USB_DISK:                  /* U�� */
            res = USBH_initialize();    /* U�̳�ʼ�� */
            break;

        default:
            res = 1;
            break;
    }

    if (res)
    {
        return  STA_NOINIT;
    }
    else
    {
        return 0; /* ��ʼ���ɹ� */
    }
}

/**
 * @brief       ������
 * @param       pdrv   : ���̱��0~9
 * @param       buff   : ���ݽ��ջ����׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫ��ȡ��������
 * @retval      ��
 */
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    uint8_t res = 0;

    if (!count)return RES_PARERR;   /* count���ܵ���0�����򷵻ز������� */

    switch (pdrv)
    {
        case USB_DISK:  /* U�� */
            res = USBH_read(buff, sector, count);
            break;

        default:
            res = 1;
            break;
    }

    /* ������ֵ��������ֵת��ff.c�ķ���ֵ */
    if (res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}

/**
 * @brief       д����
 * @param       pdrv   : ���̱��0~9
 * @param       buff   : �������ݻ������׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫд���������
 * @retval      ��
 */
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive number to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    uint8_t res = 0;

    if (!count)return RES_PARERR;   /* count���ܵ���0�����򷵻ز������� */

    switch (pdrv)
    {

        case USB_DISK:      /* U�� */
            res = USBH_write(buff, sector, count);
            break;
        default:
            res = 1;
            break;
    }

    /* ������ֵ��������ֵת��ff.c�ķ���ֵ */
    if (res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR; 
    }
}

/**
 * @brief       ��ȡ�������Ʋ���
 * @param       pdrv   : ���̱��0~9
 * @param       ctrl   : ���ƴ���
 * @param       buff   : ����/���ջ�����ָ��
 * @retval      ��
 */
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive number (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    if (pdrv == USB_DISK)  /* U�� */
    {
        res = USBH_ioctl(cmd, buff);
    }
    else
    {
        res = RES_ERROR;    /* �����Ĳ�֧�� */
    }
    
    return res;
}


