/*
 * Measure_Constants.h
 *
 *  Created on: 2022年9月1日
 *      Author: tianyu.zhao
 */


#ifndef TESTSTAND_MEASURE_CONSTANTS_H_
#define TESTSTAND_MEASURE_CONSTANTS_H_

/*消息队列相关的配置*/
#define Usart_QUEUE_LENGTH		50
#define Measure_QUEUE_LENGTH	5
#define MotorReady_QUEUE_LENGTH	1
#define SensorData_QUEUE_LENGTH 5


#define QUEUE_ITEM_SIZE			1
#define QUEUE_SENSOR_SIZE		7*sizeof(uint32_t)


/*串口打印作图区大小*/
#define X_Items 10
#define Y_Items 10


#define START_MEASURE	0x01
#define STOP_MEASURE	0x02
#define DEBUG_MOVE		0x03

#define NOTIFY_STOP_MEASURE 0x05

#define DEBUG_REV_MAX_LEN   85       /* 串口接收数据最大长度 */


//磁铁孔径的三维尺寸
#define X_LENGTH	140
#define Y_LENGTH	140
#define Z_LENGTH	10

/* LOG函数相关的缓冲长度定义*/
#define LENGTH_TIME_BUFF		30
#define LENGTH_FILE_NAME_BUFF	50
#define LENGTH_LOG_STRING_BUFF	200

/** 文件系统相关的字符串常量 */
#define MEASURE_DIR 	"0:/MagnetMeasure"
#define LOG_FILE_PATH   "0:/MagnetMeasure/SGTest.log"
extern const char StringFileDescription[];
extern const char MeasureFileSuffix[];

/** 文件系统相关的字符串常量 */


/*串口命令包的种类数*/
#define UART_COMMUNICATION_CHANNEL_COUNT	7

/*串口数据包的长度*/
#define PACK_SIZE_CONFIG		23
#define PACK_SIZE_GENERAL		7
#define PACK_SIZE_DEBUG			13
#define PACK_SIZE_TIME			14
#define PACK_SIZE_CALIBRATION	85

/*串口命令号宏定义***/
#define CMD_SEND_CONFIG		0xa0
#define CMD_READ_CONFIG		0xa1
#define CMD_SEND_START		0xa2
#define CMD_SEND_STOP		0xa3
#define CMD_SEND_DEBUG		0xa4
#define CMD_SEND_TIME		0xa7
#define CMD_SEND_CALIBRATION 0Xa8

#define COUNT_UART_CHANNELS	10
/*串口数据包的长度*/


#endif /* TESTSTAND_MEASURE_CONSTANTS_H_ */
