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
#define SensorData_QUEUE_LENGTH 5


#define QUEUE_ITEM_SIZE			1
#define QUEUE_SENSOR_SIZE		7*sizeof(uint32_t)


/*���ڴ�ӡ��ͼ����С*/
#define X_Items 10
#define Y_Items 10


#define START_MEASURE	0x01
#define STOP_MEASURE	0x02
#define DEBUG_MOVE		0x03

#define NOTIFY_STOP_MEASURE 0x05

#define DEBUG_REV_MAX_LEN   85       /* ���ڽ���������󳤶� */


//�����׾�����ά�ߴ�
#define X_LENGTH	140
#define Y_LENGTH	140
#define Z_LENGTH	10

/* LOG������صĻ��峤�ȶ���*/
#define LENGTH_TIME_BUFF		30
#define LENGTH_FILE_NAME_BUFF	50
#define LENGTH_LOG_STRING_BUFF	200

/** �ļ�ϵͳ��ص��ַ������� */
#define MEASURE_DIR 	"0:/MagnetMeasure"
#define LOG_FILE_PATH   "0:/MagnetMeasure/SGTest.log"
extern const char StringFileDescription[];
extern const char MeasureFileSuffix[];

/** �ļ�ϵͳ��ص��ַ������� */


/*�����������������*/
#define UART_COMMUNICATION_CHANNEL_COUNT	7

/*�������ݰ��ĳ���*/
#define PACK_SIZE_CONFIG		23
#define PACK_SIZE_GENERAL		7
#define PACK_SIZE_DEBUG			13
#define PACK_SIZE_TIME			14
#define PACK_SIZE_CALIBRATION	85

/*��������ź궨��***/
#define CMD_SEND_CONFIG		0xa0
#define CMD_READ_CONFIG		0xa1
#define CMD_SEND_START		0xa2
#define CMD_SEND_STOP		0xa3
#define CMD_SEND_DEBUG		0xa4
#define CMD_SEND_TIME		0xa7
#define CMD_SEND_CALIBRATION 0Xa8

#define COUNT_UART_CHANNELS	10
/*�������ݰ��ĳ���*/


#endif /* TESTSTAND_MEASURE_CONSTANTS_H_ */
