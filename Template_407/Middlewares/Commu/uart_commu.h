#ifndef __DEBUG_H
#define __DEBUG_H

#include "sys.h"
#include "Measure_Types.h"
#include <stdbool.h>
#define DEBUG_ENABLE       1         /* PID����ʹ�� */
#define EN_CRC             1         /* CRCУ�飬0���رգ�1������ */

#define DEBUG_DATA_HEAD   0x55       /* ֡ͷ */
#define DEBUG_DATA_END    0xAA       /* ֡β */

#define IS_A_PACK_FOOT(x)		(x==DEBUG_DATA_END)?true:false
#define IS_A_PACK_HEAD(x)		(x==DEBUG_DATA_HEAD)?true:false
/*********************************** ������ ����> ��λ�� *******************************************/

/* ������� */
typedef enum
{
    TYPE_ERROR          = 0x0F,      /* �������� */
    TYPE_STATUS         = 0x10,      /* �豸״̬ */
    TYPE_SPEED          = 0x11,      /* �ٶ� */
    TYPE_HAL_ENC        = 0x12,      /* ������������λ��ֵ */
    TYPE_VBUS           = 0x13,      /* ��ѹ */
    TYPE_AMP            = 0x14,      /* ���� */
    TYPE_TEMP           = 0x15,      /* �������¶� */
    TYPE_SUM_LEN        = 0x16,      /* ����� */
    TYPE_BEM            = 0x17,      /* ���綯�� */
    TYPE_MOTOR_CODE     = 0x18,      /* ������� */
    TYPE_TORQUE         = 0x19,      /* Ť�� */
    TYPE_POWER          = 0x1A,      /* ���� */
    TYPE_PID1           = 0x20,      /* �û�PID1�����ϱ� */
    TYPE_PID2           = 0x21,      /* PID2 */
    TYPE_PID3           = 0x22,      /* PID3 */
    TYPE_PID4           = 0x23,      /* PID4 */
    TYPE_PID5           = 0x24,      /* PID5 */
    TYPE_PID6           = 0x25,      /* PID6 */
    TYPE_PID7           = 0x26,      /* PID7 */
    TYPE_PID8           = 0x27,      /* PID8 */
    TYPE_PID9           = 0x28,      /* PID9 */
    TYPE_PID10          = 0x29,      /* PID10 */
    TYPE_USER_DATA      = 0x30,      /* ���������ϱ� */
}upload_type;

/* �������� */
typedef enum
{
    HALL_ENC_ERROR      = 0x01,      /* ���������������� */
    OVERSPEED_ERROR     = 0x02,      /* ������� */
    DB_OVERTEMP_ERROR   = 0x04,      /* ��������� */
    M_OVERTEMP_ERROR    = 0x08,      /* ������� */
    DB_OVERVOL_ERROR    = 0x10,      /* �������ѹ */
    DB_UNDERVOL_ERROR   = 0x10,      /* ������Ƿѹ */
    DB_OVERCUR_ERROR    = 0x10,      /* ��������� */
    UNKNOWN_ERROR       = 0x10,      /* δ֪���� */
}motor_error;

/* ���״̬ */
typedef enum
{
    IDLE_STATE          = 0x00,      /* ����״̬ */
    RUN_STATE           = 0x01,      /* ����״̬ */
    ERROR_STATE         = 0x02,      /* ����״̬ */
    LRTOUT_STATE        = 0x03,      /* ��ת��ʱ */
    BREAKED_STATE       = 0x04,      /* ɲ�� */
}motor_state;

/* ������� */
typedef enum
{
    DC_MOTOR            = 0x10,      /* ֱ����ˢ��� */
    BLDC_MOTOR          = 0x11,      /* ֱ����ˢ��� */
    PMSM_MOTOR          = 0x12,      /* ����ͬ����� */
    STEP_MOTOR          = 0x13,      /* ������� */
    SERVO_MOTOR         = 0x14,      /* �ŷ���� */
    EXCHANG_MOTOR       = 0x15,      /* ��Ƶ��(���ཻ���첽���) */
    HELM_MOTOR          = 0x16,      /* ��� */
}motor_code;

/* PID������Žṹ�� */
typedef struct
{
    union
    {
        float pidf[3];               /* PID�������ʹ�� */
        int8_t pidi8[12];            /* PID�������մ�� */
    }pid;
}pid_struct;

/* ������Žṹ�壨�����塪��>��λ���� */
typedef struct
{
    uint8_t status;                  /* ���״̬ */
    int16_t speed;                   /* ����ٶ� */
    uint8_t hall_p;                  /* ����λ��ֵ */
    uint16_t encode_p;               /* ������λ��ֵ */
    float bus_vol;                   /* ��ѹ */
    float amp[3];                    /* ���� */
    float temp[2];                   /* �¶� */
    uint64_t sum_len;                /* ����� */
    float bem[3];                    /* ���綯�� */
    uint8_t motor_code;              /* ������� */
    float torque;                    /* Ť�� */
    float power;                     /* ���� */
    pid_struct pid[10];              /* 10��PID�������շ����ã� */
    int16_t user_data[16];           /* �������� */
}debug_data;

extern debug_data g_debug;           /* ���ͱ��� */


/*********************************** ��λ��  ����>  ������ *******************************************/

/* ������� */
typedef enum
{
    CMD_GET_ALL_DATA    = 0x19,      /* ��ȡȫ������ */
    CMD_SET_CTR_CODE    = 0x21,      /* �·�����ָ�� */
    CMD_SET_CTR_MODE    = 0x22,      /* �·�����ģʽ */
    CMD_SET_SPEED       = 0x23,      /* �趨�ٶ� */
    CMD_SET_TORQUE      = 0x24,      /* �趨ת�� */
    CMD_SET_VF_VOL      = 0x25,      /* �趨VF��ѹ */
    CMD_SET_VF_IF_FRE   = 0x26,      /* �趨V/F��IFƵ�� */
    CMD_SET_IF_CUR      = 0x27,      /* �趨IF���� */
    CMD_SET_DQ_CUR_D    = 0x28,      /* �趨DQ����D */
    CMD_SET_DQ_CUR_Q    = 0x29,      /* �趨DQ����Q */
    CMD_SET_PID1        = 0x31,      /* �趨PID1���� */
    CMD_SET_PID2        = 0x32,      /* PID2 */
    CMD_SET_PID3        = 0x33,      /* PID3 */
    CMD_SET_PID4        = 0x34,      /* PID4 */
    CMD_SET_PID5        = 0x35,      /* PID5 */
    CMD_SET_PID6        = 0x36,      /* PID6 */
    CMD_SET_PID7        = 0x37,      /* PID7 */
    CMD_SET_PID8        = 0x38,      /* PID8 */
    CMD_SET_PID9        = 0x39,      /* PID9 */
    CMD_SET_PID10       = 0x3A,      /* PID10 */
}cmd_type;

/* �·�����ָ�� */
typedef enum
{
    HALT_CODE           = 0x01,      /* ͣ�� */
    RUN_CODE            = 0x02,      /* ���� */
    BREAKED             = 0x03,      /* ɲ�� */
}cmd_code;

/* �·�����ģʽ */
typedef enum
{
    SPEED_MODE          = 0x01,      /* ת��ģʽ */
    TORQUE_MODE         = 0x02,      /* ת��ģʽ */
    IF_MODE             = 0x03,      /* IFģʽ */
    VF_MODE             = 0x04,      /* VFģʽ */
    DQ_MODE             = 0x05,      /* DQģʽ */
}cmd_mode;

/* ������Žṹ�壨��λ������>�����壩 */
typedef struct
{
    uint8_t Ctrl_code;
    uint8_t Ctrl_mode;
    float *speed;
    float *torque;
    float pid[3];
}debug_data_rev;

extern debug_data_rev debug_rev;     /* ���ձ��� */



/*****************************************************************************************************/

/* �ײ��������� */
void debug_obj_init(debug_data *data);                                              /* �ڴ��ʼ�� */
bool debug_handle(uint8_t *data, MeasureConfig_t *pConfig);                                                   /* ��λ�����ݽ��� */
void debug_upload_data(debug_data * data, uint8_t upload_type);                     /* �����ϴ� */

/* Ӧ�ò㺯�� */
void debug_init(void);                                                              /* ��ʼ������ */
void debug_set_point_range(float max_limit,float min_limit,float step_max);         /* ����Ŀ���ٶȷ�Χ */

void debug_send_initdata(upload_type PIDx,float *SetPoint,float P,float I,float D); /* PID��ʼ�������ϴ� */
void debug_send_current(float U_I,float V_I,float W_I);                             /* ���������ϴ� */
void debug_send_valtage(float valtage);                                             /* ��ѹ�����ϴ� */
void debug_send_power(float power);                                                 /* ���������ϴ� */
void debug_send_speed(float speed);                                                 /* �ٶ������ϴ� */
void debug_send_distance(uint64_t len);                                             /* ����������ϴ� */
void debug_send_temp(float motor_temp,float board_temp);                            /* �¶������ϴ� */
void debug_send_motorstate(motor_state motor_codestae);                             /* ���״̬�ϴ� */
void debug_send_motorcode(motor_code motorcode);                                    /* ��������ϴ� */
void debug_send_wave_data(uint8_t chx,int16_t wave);                                /* ���������ϴ� */

void debug_receive_pid(upload_type PIDx,float *P,float *I,float *D);                /* ��λ��PID�������� */
uint8_t debug_receive_ctrl_code(void);                                              /* ��λ��������� */

void debug_structSize();

/***********���ڰ���������***********/
void config_parse_handler(void *arg, uint8_t index);
void debug_parse_handler(void *arg, uint8_t index);
void time_parse_handler(void *arg, uint8_t index);
/***********���ڰ���������***********/
#endif


