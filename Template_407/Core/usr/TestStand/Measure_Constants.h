/*
 * Measure_Constants.h
 *
 *  Created on: 2022��9��1��
 *      Author: tianyu.zhao
 */


#ifndef TESTSTAND_MEASURE_CONSTANTS_H_
#define TESTSTAND_MEASURE_CONSTANTS_H_

/*��Ϣ������ص�����*/
#define Usart_QUEUE_LENGTH		50
#define Measure_QUEUE_LENGTH	5
#define MotorReady_QUEUE_LENGTH	1

#define QUEUE_SIZE				1

/*���ڴ�ӡ��ͼ����С*/
#define X_Items 10
#define Y_Items 10


#define START_MEASURE	0x01
#define STOP_MEASURE	0x02
#define DEBUG_MOVE		0x03

#define NOTIFY_STOP_MEASURE 0x05

#define DEBUG_REV_MAX_LEN   23       /* ���ڽ���������󳤶� */


//�����׾�����ά�ߴ�
#define X_LENGTH	100
#define Y_LENGTH	10
#define Z_LENGTH	70



/** �ļ�ϵͳ��ص��ַ������� */
#define MEASURE_DIR 	"0:/MagnetMeasure"
#define LOG_FILE_PATH   "0:/MagnetMeasure/SGTest.log"
/** �ļ�ϵͳ��ص��ַ������� */


/*�������ݰ��ĳ���*/
#define PACK_SIZE_CONFIG	23
#define PACK_SIZE_GENERAL	7
#define PACK_SIZE_DEBUG		13
#define PACK_SIZE_TIME		14

/*��������ź궨��***/
#define CMD_SEND_CONFIG		0xa0
#define CMD_READ_CONFIG		0xa1
#define CMD_SEND_START		0xa2
#define CMD_SEND_STOP		0xa3
#define CMD_SEND_DEBUG		0xa4
#define CMD_SEND_TIME		0xa4

#define COUNT_UART_CHANNELS	10
/*�������ݰ��ĳ���*/

#endif /* TESTSTAND_MEASURE_CONSTANTS_H_ */