/**
 ****************************************************************************************************
 * @file        usbh_diskio.h
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
 * V1.0 200211126
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __USBH_DISKIO_H
#define __USBH_DISKIO_H

#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"


#define USB_DEFAULT_BLOCK_SIZE      512


DSTATUS USBH_initialize(void);
DSTATUS USBH_status(void);
DRESULT USBH_read(BYTE *buff, DWORD sector, UINT count);
DRESULT USBH_write(const BYTE *buff,DWORD sector,UINT count);
DRESULT USBH_ioctl(BYTE cmd,void *buff);

#endif
