/**
 ****************************************************************************************************
 * @file        ffsystem.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-14
 * @brief       FATFS底层(ffsystem) 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台：正点原子 STM32开发板
 * 在线视频：www.yuanzige.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 *
 * 修改说明
 * V1.0 20220114
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "malloc.h"
#include "sys.h"
#include "ff.h"
#include "rtc.h"

/**
 * @brief       获得时间
 * @param       mf  : 内存首地址
 * @retval      时间
 *   @note      时间编码规则如下:
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
 * @brief       动态分配内存
 * @param       size : 要分配的内存大小(字节)
 * @retval      分配到的内存首地址.
 */
void *ff_memalloc (UINT size)
{
    return (void*)malloc(size);
}

/**
 * @brief       释放内存
 * @param       mf  : 内存首地址
 * @retval      无
 */
void ff_memfree (void* mf)
{
    free(mf);
}

















