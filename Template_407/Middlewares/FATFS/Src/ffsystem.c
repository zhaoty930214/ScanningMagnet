/**
 ****************************************************************************************************
 * @file        ffsystem.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-14
 * @brief       FATFS�ײ�(ffsystem) ��������
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
 *
 ****************************************************************************************************
 */

#include "malloc.h"
#include "sys.h"
#include "ff.h"
#include "rtc.h"

/**
 * @brief       ���ʱ��
 * @param       mf  : �ڴ��׵�ַ
 * @retval      ʱ��
 *   @note      ʱ������������:
 *              User defined function to give a current time to fatfs module 
 *              31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)
 *              15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) 
 */
DWORD get_fattime (void)
{
	DWORD ret;
	uint8_t year, month, date, week;
    rtc_get_date(&year, &month, &date, &week);

	uint8_t hour, min, sec, ampm;
	rtc_get_time(&hour, &min, &sec, &ampm);

	ret = (year+20)<<25 |
		  (month<<21)	|
		  (date<<16)	|
		  (hour<<11)	|
		  (min<<5)		|
		  sec>>1;


    return ret;
}

/**
 * @brief       ��̬�����ڴ�
 * @param       size : Ҫ������ڴ��С(�ֽ�)
 * @retval      ���䵽���ڴ��׵�ַ.
 */
void *ff_memalloc (UINT size)
{
    return (void*)malloc(size);
}

/**
 * @brief       �ͷ��ڴ�
 * @param       mf  : �ڴ��׵�ַ
 * @retval      ��
 */
void ff_memfree (void* mf)
{
    free(mf);
}

















