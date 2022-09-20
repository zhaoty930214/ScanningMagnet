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

#define QUEUE_SIZE				1

/*串口打印作图区大小*/
#define X_Items 10
#define Y_Items 10


#define START_MEASURE	0x01
#define STOP_MEASURE	0x02
#define DEBUG_MOVE		0x03

#define NOTIFY_STOP_MEASURE 0x05

#endif /* TESTSTAND_MEASURE_CONSTANTS_H_ */
