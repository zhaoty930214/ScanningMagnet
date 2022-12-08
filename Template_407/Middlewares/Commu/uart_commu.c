#include "uart_commu.h"
#include "my_usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart_types.h"
#include "LED.h"
#include "freertos.h"
#include "task.h"
#include "Measure_Types.h"
#include "my_tasks.h"
#include "rtc.h"
/*************************************** ��һ���� CRCУ�� ********************************************/

/* CRC ��λ�ֽ�ֵ�� */
static const uint8_t s_crchi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC ��λ�ֽ�ֵ�� */
const uint8_t s_crclo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/**
 * @brief       CRC16У��
 * @param       *_pBuf�����ݵ�ַ
 * @param       _usLen�����ݳ���
 * @note        ����crc16-modbus������ʽhex��8005
 * @retval      У����
 */
uint16_t crc16_calc(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucCRCHi = 0xFF;                         /* ��CRC�ֽڳ�ʼ�� */
    uint8_t ucCRCLo = 0xFF;                         /* ��CRC �ֽڳ�ʼ�� */
    uint16_t usIndex;                               /* CRCѭ���е����� */

    while (_usLen--)
    {
        usIndex = ucCRCLo ^ *_pBuf++;               /* ����CRC */
        ucCRCLo = ucCRCHi ^ s_crchi[usIndex];
        ucCRCHi = s_crclo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);      /* ����У���������ֽ��ڸ�λ */
}


/*************************************** �ڶ����� �ײ㺯�� ********************************************/

__IO uint8_t debug_rev_data[DEBUG_REV_MAX_LEN];     /* ��Ž������ݵ����飨���λ������� */
__IO uint8_t debug_rev_p = 0;                       /* ��ַƫ���� */

debug_data g_debug;
debug_data_rev debug_rev;

/**
 * @brief       �ڴ��ʼ��
 * @param       *data���ڴ���ʼ��ַ
 * @retval      ��
 */
void debug_obj_init(debug_data *data)
{
    size_t obj_size = sizeof(debug_data);
    memset(data, 0, (size_t)obj_size);             /* ��ָ����Χ�ڴ����� */
}



void debug_structSize()
{
	printf("Size of char is %d\r\n", sizeof(unsigned char));
	printf("Size of short is %d\r\n", sizeof(unsigned short));
	printf("Size of MsgHeader_t is %d\r\n", sizeof(MsgHeader));
	printf("Size of MsgFooter_t is %d\r\n", sizeof(MsgFooter));
	printf("Size of SendConfig_t is %d\r\n", sizeof(SendConfig));
	printf("Version 1.0.0.1\r\n");

	uint32_t int1 = 0x3c23d70a;

	float float1 = *(float*) (&int1);

	printf("Float value is %f\r\n", float1);

	printf("Size of MeasureConfig_t is %d\r\n", sizeof(MeasureConfig_t));
}



void debug_receive_data(uint8_t *data)
{

}

float bytes2float(__IO uint8_t *pdata1)
{
	float ret = 0;
	uint32_t int1 = 0;
	int1 = *pdata1  		|
		   *(pdata1+1) << 8 |
		   *(pdata1+2) << 16|
		   *(pdata1+3) << 24;

	ret = *(float*) (&int1);

	return ret;
}





extern const Uart_Struct_t g_uart_handle_struct[6];




/*debug_handle���������ﴫ�붥��ͨ�ýṹ��ָ�룬
 *���ڴ��������������С�˸�ʽ����Ҫת�ɴ�˸�ʽ�ٴ���
 *������������ʹ�õĽ����ص�����
 */
void config_parse_handler(void *arg, uint8_t index)
{
	uint8_t *pdata = arg;
	MeasuringConfig_t *pconfig_t = (MeasuringConfig_t*) g_uart_handle_struct[index].pstorage;

	pconfig_t->step_x = bytes2float(pdata);
	pconfig_t->step_y = bytes2float(pdata+4);
	pconfig_t->step_z = bytes2float(pdata+8);
	pconfig_t->amplifier_current = bytes2float(pdata+12);

	printf("%s, %d\r\n", __FILE__, __LINE__);
	printf("config_parse_handler: %f, %f, %f, %f\r\n",
			pconfig_t->step_x,
			pconfig_t->step_y,
			pconfig_t->step_z,
			pconfig_t->amplifier_current);
}


/*debug_handle���������ﴫ�붥��ͨ�ýṹ��ָ�룬
 *���ڴ��������������С�˸�ʽ����Ҫת�ɴ�˸�ʽ�ٴ���
 *���͵�������ʹ�õĽ����ص�����
 */
void debug_parse_handler(void *arg, uint8_t index)
{
	uint8_t *pdata = arg;
	DebugConfig_t *pdebug_t = (DebugConfig_t*) g_uart_handle_struct[index].pstorage;

	pdebug_t->axis_id = *pdata;
	pdebug_t->direction = *(pdata+1);
	pdebug_t->distance = bytes2float(pdata+2);

	printf("%s, %d\r\n", __FILE__, __LINE__);
	printf("debug_parse_handler: Axis id:%c, direction:%s, distance:%2.2f \r\n",
			pdebug_t->axis_id == 0x04?'X':(pdebug_t->axis_id == 0x05?'Y':'Z'),
			pdebug_t->direction == 0x01? "Left":"Right",
			pdebug_t->distance);

}


/**
 * debug_handle���������ﴫ�붥��ͨ�ýṹ��ָ��
 */
void time_parse_handler(void *arg, uint8_t index)
{
	uint8_t *pdata = arg;
	TimeConfig_t *ptime_t = (TimeConfig_t*) g_uart_handle_struct[index].pstorage;

	ptime_t->year	= (*pdata) | *(pdata+1)<<8;
	ptime_t->month	= *(pdata+2);
	ptime_t->day	= *(pdata+3);
	ptime_t->hour   = *(pdata+4);
	ptime_t->minute = *(pdata+5);
	ptime_t->second = *(pdata+6);

	printf("Got time %04d-%02d-%02d, %02d:%02d:%02d \r\n",
			ptime_t->year,
			ptime_t->month,
			ptime_t->day,
			ptime_t->hour,
			ptime_t->minute,
			ptime_t->second );

	rtc_set_date(ptime_t->year-2000, ptime_t->month, ptime_t->day, 1);
	rtc_set_time(ptime_t->hour, ptime_t->minute, ptime_t->second, RTC_HOURFORMAT12_PM);

	uint8_t year, month, date, week;
    rtc_get_date(&year, &month, &date, &week);
    printf("Date:20%02d-%02d-%02d\r\n", year, month, date);

	volatile uint8_t hour, min, sec, milisec,  ampm;
	rtc_get_time(&hour, &min, &sec, &milisec, &ampm);
	printf("Time:%02d:%02d:%02d\r\n", hour, min, sec);
}


//void




/**
 * @brief       ��λ�����ݽ���
 * @param       *data�����յ����ݣ���ַ��
 * @note        ���û��λ��������������ݣ��ٴ����Ӧ�Ľṹ���Ա��
 * @retval      ��
 */
bool debug_handle(uint8_t *data, MeasureConfig_t *pConfig)
{
	bool ret_val = false;
	uint8_t *pdata;
	static uint8_t size_rev_max_len = sizeof(SendConfig);
    //uint8_t i;

    if (debug_rev_p >= size_rev_max_len)          /* ���������������飩��󳤶� */
    {
        debug_rev_p = 0;                           /* ��ַƫ�������� */
    }

    debug_rev_data[debug_rev_p] = *(data);         /* ȡ�����ݣ�������� */
    __IO uint8_t pShift_debug = debug_rev_p + DEBUG_REV_MAX_LEN;	/*���һ�����ݵ�������*/

    if(IS_A_PACK_FOOT(*data))
    {
    	//����Э�����ݣ�����̰�����ʼ����
    	/*�ܰ���Ϊ7, ��ֻ����һ��ͨ����*/

    	for(int i=0; i<10; i++)
    	{
    		uint8_t pack_head_index = pShift_debug - g_uart_handle_struct[i].pack_length+1;
        	if(IS_A_PACK_HEAD(debug_rev_data[pack_head_index%DEBUG_REV_MAX_LEN]) )
        	{
        		ret_val = true;
        		printf("channel id is %d\r\n", debug_rev_data[(pack_head_index+3)%DEBUG_REV_MAX_LEN]);
        		pConfig->cmd_channel = 0xa0 + debug_rev_data[(pack_head_index+3)%DEBUG_REV_MAX_LEN];

        		/*�жϻ��ν��ջ����У���ͷ����β�Ƿ�˳������*/
        		uint8_t index_start, index_end;

        		index_start = pack_head_index%DEBUG_REV_MAX_LEN;
        		index_end   = debug_rev_p;
        		//�����ʼ�����Ŵ��ڽ�β�����ţ���˵�����λ������Ƕ�����
        		//��ʱ��Ҫ�����ݰ��մ�ͷ��β��˳�򣬱��浽�����У���ָ��������á�
        		if(index_start > index_end)
        		{
        			uint8_t size_front, size_behind;
        			size_front  = DEBUG_REV_MAX_LEN - index_start;
        			size_behind = debug_rev_p+1;

        			printf("start index %d\r\n", index_start);
        			printf("end index %d\r\n", index_end);

        			memcpy(&pConfig->recv_data[0], &debug_rev_data[index_start], size_front);
        			memcpy(&pConfig->recv_data[size_front], &debug_rev_data[0], size_behind);
        			memset(&debug_rev_data[0], 0, DEBUG_REV_MAX_LEN);
        			//gUartHandler[i].pstorage = &pConfig->recv_data[0];


        			for(int j=0; j<(size_front+size_behind); j++)
        			{
        				printf("DEBUG %d dest data is 0x%x . oridata is 0x%x\r\n",
        						j, pConfig->recv_data[j], debug_rev_data[j]);
        			}
        		}
        		else
        		{
        			memcpy(&pConfig->recv_data[0], &debug_rev_data[pack_head_index%DEBUG_REV_MAX_LEN], g_uart_handle_struct[i].pack_length);
        			memset(&debug_rev_data[0], 0, DEBUG_REV_MAX_LEN);

        			//û��Ƕ�ף���ֱ��ָ�򴮿ڻ���
        			//gUartHandler[i].pstorage = &debug_rev_data[(pShift_debug - gUartHandler[i].pack_size+1+4)%DEBUG_REV_MAX_LEN];

        		}

        		if(g_uart_handle_struct[i].data_handler != NULL)
        		{
        			pdata = &pConfig->recv_data[0+4];
        			g_uart_handle_struct[i].data_handler(pdata, i);
        		}
        		break;
        	}
    	}
    }
    else
    {
    	ret_val = false;
    	//printf("It's not a pack foot 0x%x\r\n", *data);
    }
    debug_rev_p++;

    return ret_val;
}

/**
 * @brief       �����ϴ�
 * @param       *data���ϴ������ݣ���ַ��
 * @param       upload_type���ϴ����������
 * @retval      ��
 */
void debug_upload_data(debug_data *data, uint8_t upload_type)
{
    uint8_t cur_data, i;
    uint8_t upload_data[37];                                            /* �����ϴ����� */
    upload_data[0] = DEBUG_DATA_HEAD;                                   /* ���ݰ���1���ֽڣ������0��Ԫ�أ����̶�Ϊ֡ͷ */
    cur_data = 2;                                                       /* ������ӵ�3���ֽڣ������2��Ԫ�أ���ʼ */

    switch (upload_type)                                                /* �ж�������� */
    {
        case TYPE_STATUS:                                               /* �豸״̬ */
            upload_data[1] = upload_type;                               /* ���ݰ���2���ֽڣ������1��Ԫ�أ����̶�Ϊ������� */
            upload_data[cur_data++] = data->status;                     /* ����Ҫ���͵������� */
            break;

        case TYPE_SPEED:                                                /* ����ٶ� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (data->speed >> 8) & 0xFF;        /* �ȴ����ٶ�ֵ��8λ��С��ģʽ��u16����u8���ͣ�ֻȡ��8λ�� */
            upload_data[cur_data++] = data->speed & 0xFF;               /* �ٴ����ٶ�ֵ��8λ */
            break;

        case TYPE_HAL_ENC:                                              /* ������������λ��ֵ */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (data->hall_p) & 0x08;            /* �������λ��ֵ */
            upload_data[cur_data++] = (data->encode_p >> 8) & 0xFF;     /* ���������λ��ֵ��8λ */
            upload_data[cur_data++] = (data->encode_p) & 0xFF;          /* ���������λ��ֵ��8λ */
            break;

        case TYPE_VBUS:                                                                 /* ��ѹ����Χ 0~100.99 V */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = ((uint8_t)data->bus_vol) % 101;                   /* �����ѹֵ�������֣��������ֲ�������100 */
            upload_data[cur_data++] = ((uint16_t)(data->bus_vol * 100)) % 100;          /* �����ѹֵС�����֣�С�����ֲ�������99 */
            break;

        case TYPE_AMP:                                                                  /* ���� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (((int16_t)(data->amp[0] * 1000)) >> 8) & 0xFF;   /* ����U�������8λ */
            upload_data[cur_data++] = ((int16_t)(data->amp[0] * 1000)) & 0xFF;          /* ����U�������8λ */
            upload_data[cur_data++] = (((int16_t)(data->amp[1] * 1000)) >> 8) & 0xFF;   /* ����V�������8λ */
            upload_data[cur_data++] = ((int16_t)(data->amp[1] * 1000)) & 0xFF;          /* ����V�������8λ */
            upload_data[cur_data++] = (((int16_t)(data->amp[2] * 1000)) >> 8) & 0xFF;   /* ����W�������8λ */
            upload_data[cur_data++] = ((int16_t)(data->amp[2] * 1000)) & 0xFF;          /* ����W�������8λ */
            break;

        case TYPE_TEMP:                                                                 /* �¶� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (uint8_t)(data->temp[0] + 50);                    /* �����������¶� */
            upload_data[cur_data++] = (uint8_t)(data->temp[1] + 50);                    /* �������¶� */
            break;

        case TYPE_SUM_LEN:                                                              /* ����� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (data->sum_len >> 56) & 0xFF;                     /* ��������� 56~63 λ */
            upload_data[cur_data++] = (data->sum_len >> 48) & 0xFF;                     /* ��������� 48~55 λ */
            upload_data[cur_data++] = (data->sum_len >> 40) & 0xFF;                     /* ��������� 40~47 λ */
            upload_data[cur_data++] = (data->sum_len >> 32) & 0xFF;                     /* ��������� 32~39 λ */
            upload_data[cur_data++] = (data->sum_len >> 24) & 0xFF;                     /* ��������� 24~31 λ */
            upload_data[cur_data++] = (data->sum_len >> 16) & 0xFF;                     /* ��������� 16~23 λ */
            upload_data[cur_data++] = (data->sum_len >> 8) & 0xFF;                      /* ��������� 8~15 λ */
            upload_data[cur_data++] = (data->sum_len >> 0) & 0xFF;                      /* ��������� 0~7 λ */
            break;

        case TYPE_BEM:                                                                  /* ���綯�� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (int8_t)data->bem[0];                             /* ����U�෴�綯�Ƶ�ѹ�������� */
            upload_data[cur_data++] = ((int16_t)(data->bem[0] * 100)) % 100;            /* ����U�෴�綯�Ƶ�ѹС������ */
            upload_data[cur_data++] = (int8_t)data->bem[1];                             /* ����V�෴�綯�Ƶ�ѹ�������� */
            upload_data[cur_data++] = ((int16_t)(data->bem[1] * 100)) % 100;            /* ����V�෴�綯�Ƶ�ѹС������ */
            upload_data[cur_data++] = (int8_t)data->bem[2];                             /* ����W�෴�綯�Ƶ�ѹ�������� */
            upload_data[cur_data++] = ((int16_t)(data->bem[2] * 100)) % 100;            /* ����W�෴�綯�Ƶ�ѹС������ */
            break;

        case TYPE_MOTOR_CODE:                                                           /* ������� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = data->motor_code;                                 /* ���������� */
            break;

        case TYPE_TORQUE:                                                               /* Ť�� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (((int16_t)(data->torque * 1000)) >> 8) & 0xFF;   /* ����Ť��ֵ�������� */
            upload_data[cur_data++] = ((int16_t)(data->torque * 1000)) & 0xFF;          /* ����Ť��ֵС������ */
            break;

        case TYPE_POWER:                                                                /* ���� */
            upload_data[1] = upload_type;
            upload_data[cur_data++] = (((int16_t)(data->power * 100)) >> 8) & 0xFF;     /* ���빦��ֵ��8λ */
            upload_data[cur_data++] = ((int16_t)(data->power * 100)) & 0xFF;            /* ���빦��ֵ��8λ */
            break;

        case TYPE_PID1:                                                                 /* PID������� */
        case TYPE_PID2:
        case TYPE_PID3:
        case TYPE_PID4:
        case TYPE_PID5:
        case TYPE_PID6:
        case TYPE_PID7:
        case TYPE_PID8:
        case TYPE_PID9:
        case TYPE_PID10:
            upload_data[1] = upload_type;

            for (i = 0; i < 3; i++)                                            /* ѭ������P��I��Dϵ��ֵ��ÿ��ϵ��ռ4���ֽ� */
            {
                upload_data[cur_data++] = data->pid[upload_type - TYPE_PID1].pid.pidi8[i * 4 + 0];
                upload_data[cur_data++] = data->pid[upload_type - TYPE_PID1].pid.pidi8[i * 4 + 1];
                upload_data[cur_data++] = data->pid[upload_type - TYPE_PID1].pid.pidi8[i * 4 + 2];
                upload_data[cur_data++] = data->pid[upload_type - TYPE_PID1].pid.pidi8[i * 4 + 3];
            }
            break;

        case TYPE_USER_DATA:                                                   /* �������� */
            upload_data[1] = upload_type;

            for (i = 0; i < 16; i++)                                           /* ѭ������1~16��ͨ���������� */
            {
                upload_data[cur_data++] = (data->user_data[i] >> 8) & 0xFF;    /* ���벨�����ݸ�8λ */
                upload_data[cur_data++] =  data->user_data[i] & 0xFF;          /* ���벨�����ݵ�8λ */
            }
            break;

        default :
            upload_data[1] = 0xFE;                                             /* ���������󣬴��������0xFE */
            break;
    }

    if (upload_data[1] == 0xFE)                                                /* ����������ֱ������ */
    {
        return;
    }
    else                                                                       /* ���������ȷ */
    {
        uint16_t crc_res = crc16_calc(&(upload_data[0]), cur_data);            /* ����CRCУ�� */
        upload_data[cur_data++] = (crc_res >> 8) & 0xFF;                       /* ����У������8λ */
        upload_data[cur_data++] = (crc_res) & 0xFF;                            /* ����У������8λ */
        upload_data[cur_data++] = DEBUG_DATA_END;                              /* ����֡β */
        HAL_UART_Transmit(&g_uart1_handle, upload_data, cur_data, 0xFFFF);     /* �������ݵ���λ�� */
    }
}


/*************************************** �������� Ӧ�ò㺯�� ********************************************/

/**
 * @brief       ��ʼ������
 * @param       ��
 * @retval      ��
 */
void debug_init(void)
{
    debug_obj_init(&g_debug);            /* ��ʼ�������ڴ� */
}

/**
 * @brief       ����Ŀ���ٶȷ�Χ
 * @param       max_limit�����ֵ
 * @param       min_limit����Сֵ����תʱ����ٶȣ�
 * @param       step_max �� ���ͻ��ֵ
 * @retval      ��
 */
void debug_set_point_range(float max_limit, float min_limit, float step_max)
{
    static float step_temp = 0.0;

    if (abs((int)(*debug_rev.speed - step_temp)) > step_max)     /* �ж��ٶ�ͻ���Ƿ񳬹�����Χ */
    {
        *debug_rev.speed = step_temp;                            /* �������ͻ��ֵ������ԭ���ٶ� */
    }

    step_temp = *debug_rev.speed;                                /* ���汾���ٶ� */

    if (*debug_rev.speed >= max_limit)                           /* �������� */
    {
        *debug_rev.speed = max_limit;                            /* ����Ϊ��������ٶ� */
    }

    if (*debug_rev.speed <= min_limit)                           /* �������� */
    {
        *debug_rev.speed = min_limit;                            /* ����Ϊ��������ٶ� */
    }
}

/*************************************** ������ ����> ��λ�� ********************************************/

/**
 * @brief       PID�����ϴ�
 * @param       PIDx      ��PID�飨1~10��
 * @param       *SetPoint ��Ŀ���ٶȵ�ַ
 * @param       P��I��D   ��PID����
 * @retval      ��
 */
void debug_send_initdata(upload_type PIDx, float *SetPoint, float P, float I, float D)
{
    debug_rev.speed = (float *)(SetPoint);          /* ���������λ������һ��PIDĿ��ֵ���ڴ��ַ������ͬ�������� */

    g_debug.pid[PIDx - TYPE_PID1].pid.pidf[0] = P;  /* ����Pֵ */
    g_debug.pid[PIDx - TYPE_PID1].pid.pidf[1] = I;  /* ����Iֵ */
    g_debug.pid[PIDx - TYPE_PID1].pid.pidf[2] = D;  /* ����Dֵ */
    debug_upload_data(&g_debug, PIDx);              /* ����PID���� */
}

/**
 * @brief       ���������ϴ�
 * @param       U_I��V_I��W_I �������������
 * @note        ���ֻ�е��࣬ϰ����U_I�ϴ�
 * @retval      ��
 */
void debug_send_current(float U_I, float V_I, float W_I)
{
    g_debug.amp[0] = U_I;                           /* ����U�����ֵ */
    g_debug.amp[1] = V_I;                           /* ����V�����ֵ */
    g_debug.amp[2] = W_I;                           /* ����W�����ֵ */
    debug_upload_data(&g_debug, TYPE_AMP);          /* ���͵������� */
}

/**
 * @brief       ��ѹ�����ϴ�
 * @param       valtage ����ѹ����
 * @retval      ��
 */
void debug_send_valtage(float valtage)
{
    g_debug.bus_vol = valtage;                      /* �����ѹֵ */
    debug_upload_data(&g_debug, TYPE_VBUS);         /* ���͵�ѹ���� */
}

/**
 * @brief       ���������ϴ�
 * @param       power ����������
 * @retval      ��
 */
void debug_send_power(float power)
{
    g_debug.power = power;                          /* ���빦��ֵ */
    debug_upload_data(&g_debug, TYPE_POWER);        /* ���͹������� */
}

/**
 * @brief       �ٶ������ϴ�
 * @param       speed ���ٶ�����
 * @retval      ��
 */
void debug_send_speed(float speed)
{
    g_debug.speed = (int16_t)(speed);               /* �����ٶ�ֵ */
    debug_upload_data(&g_debug, TYPE_SPEED);        /* �����ٶ����� */
}

/**
 * @brief       ����������ϴ�
 * @param       len �����������
 * @retval      ��
 */
void debug_send_distance(uint64_t len)
{
    g_debug.sum_len = len;                          /* ���������ֵ */
    debug_upload_data(&g_debug, TYPE_SUM_LEN);      /* ������������� */
}

/**
 * @brief       �¶������ϴ�
 * @param       motor_temp ������¶�
 * @param       board_temp ���������¶�
 * @retval      ��
 */
void debug_send_temp(float motor_temp, float board_temp)
{
    g_debug.temp[0] = board_temp;                   /* �����������¶�ֵ */
    g_debug.temp[1] = motor_temp;                   /* �������¶�ֵ */
    debug_upload_data(&g_debug, TYPE_TEMP);         /* �����¶����� */
}

/**
 * @brief       ���״̬�ϴ�
 * @param       motor_codestae �����״̬
 * @retval      ��
 */
void debug_send_motorstate(motor_state motor_codestae)
{
    g_debug.status = motor_codestae;                /* ������״̬ */
    debug_upload_data(&g_debug, TYPE_STATUS);       /* ���͵��״̬ */
}

/**
 * @brief       ��������ϴ�
 * @param       motorcode ���������
 * @retval      ��
 */
void debug_send_motorcode(motor_code motorcode)
{
    g_debug.motor_code = motorcode;                 /* ���������� */
    debug_upload_data(&g_debug, TYPE_MOTOR_CODE);   /* ���͵������ */
}

/**
 * @brief       ���������ϴ�
 * @param       chx ��ͨ����ȡֵ1~16
 * @param       wave������
 * @retval      ��
 */
void debug_send_wave_data(uint8_t chx, int16_t wave)
{
    g_debug.user_data[chx - 1] = wave;              /* ѡ��ͨ������������ */
    debug_upload_data(&g_debug, TYPE_USER_DATA);    /* ���Ͳ������� */
}

/*************************************** ��λ�� ����> ������ ********************************************/

/**
 * @brief       ��λ��PID��������
 * @param       PIDx    ��PID�飨1~10��
 * @param       P��I��D ��PID����
 * @retval      ��
 */
void debug_receive_pid(upload_type PIDx, float *P, float *I, float *D)
{
    *P = g_debug.pid[PIDx - TYPE_PID1].pid.pidf[0]; /* ����P���� */
    *I = g_debug.pid[PIDx - TYPE_PID1].pid.pidf[1]; /* ����I���� */
    *D = g_debug.pid[PIDx - TYPE_PID1].pid.pidf[2]; /* ����D���� */
}

/**
 * @brief       ��λ���������
 * @param       ��
 * @retval      0����Ч��1��ͣ����2�����У�3��ɲ��
 */
uint8_t debug_receive_ctrl_code(void)
{
    static uint8_t rec_r = 0;
    if (debug_rev.Ctrl_code >= 0x01 && debug_rev.Ctrl_code <= 0x03) /* �ж����Χ�Ƿ���ȷ */
    {
        rec_r++;
        if (rec_r >= 2)
        {
            rec_r = 0;
            debug_rev.Ctrl_code = 0;
        }
        return debug_rev.Ctrl_code;                 /* �������� */
    }
    return 0;
}




