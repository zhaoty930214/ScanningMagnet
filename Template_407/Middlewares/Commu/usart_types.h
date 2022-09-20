#ifndef __USART_TYPES_H
#define __USART_TYPES_H

typedef struct MsgHeader_t{
	unsigned char  pack_start;
	unsigned short pack_length;
	unsigned char  pack_channel;
} __attribute__((packed)) MsgHeader;


typedef struct MsgFooter_t{
	unsigned short pack_crc;
	unsigned char  pack_tail;
} __attribute__((packed)) MsgFooter;


struct SendConfig_t{
	MsgHeader header;
	float data[4];
	MsgFooter footer;
} __attribute__((packed)) SendConfig;


#endif
