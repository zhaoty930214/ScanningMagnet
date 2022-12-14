#include "SGLog.h"
#include "ff.h"
#include "rtc.h"
#include "fattester.h"
#include "SGLog_Constants.h"

static FIL *s_pfile;
static FRESULT s_res;
static bool s_isInitialized = false;
static FILINFO s_fno;

void log_init(void)
{
	s_pfile = (FIL *)malloc(sizeof(FIL));
	if(s_pfile == NULL)
	{
		printf("malloc failed \r\n");
		return;
	}

	if( /*(s_res != FR_EXIST) && */  (s_res != FR_OK) )
	{
		free(s_pfile);
		printf("Open log file failed res %d\r\n", s_res);
		return;
	}

	s_isInitialized = true;
}


char *log_title[7]=
{
	"INVALID",
    "FATAL",
	"CRITICAL",
	"ERROR",
	"WARNING",
	"INFO",
	"DEBUG"
};

void open_log_file(void)
{
	/** 检查LOG文件状态， 是否已有该文件 */
	s_res = f_stat(LOG_FILE_PATH, &s_fno);

	/** 没有则创建 */
	if(FR_NO_FILE == s_res)
	{
		s_res = f_open(s_pfile, LOG_FILE_PATH, FA_CREATE_NEW |
											   FA_WRITE 	 |
											   FA_READ);
		printf("Log file not exists, create a new\r\n");
	}
	/** 有则追加方式打开 */
	else if( FR_OK == s_res)
	{
		s_res = f_open(s_pfile, LOG_FILE_PATH, FA_OPEN_APPEND |
											   FA_WRITE 	  |
											   FA_READ);
#if DEBUG_MODE
		printf("Log file exists, open and append\r\n");
#endif
	}
	else
	{
#if DEBUG_MODE
		printf("Log file stat %d\r\n", s_res);
#endif
	}

}

void close_log_file(void)
{
	if(s_pfile != NULL)
		f_close(s_pfile);
}


/*从文件件路径中获取文件名
 *
 */
void GetFileName(char *path,char *filename)
{
    char *ptr = NULL;
    ptr = strrchr(path,'/');
    if (!ptr)
        return;
    memcpy(filename,ptr+1,strlen(ptr+1));

    /* 文件名长度没有填满的情况下，在结尾添加字符串结束符*/
    if(strlen(filename) < LENGTH_FILE_NAME_BUFF)
    	strcat(filename, "\0");
}


void get_time_string(char *buff)
{
	uint8_t year, month, date, week;
	uint8_t hour, min, sec, ampm=0;
	uint16_t milisec;
	rtc_get_time(&hour, &min, &sec, &milisec, &ampm);
	rtc_get_date(&year, &month, &date, &week);
#if DEBUG_MODE
	printf("get_time_string:%04d-%02d-%02d %02d:%02d:%02d\r\n",
			year+2000, month, date, hour, min, sec);
#endif
	sprintf(buff,  "%04d-%02d-%02d %02d:%02d:%02d:%03d",
			year+2000, month, date, hour, min, sec, milisec);
}

void logMessage(
			EventLogSeverity_t severity,
			char *file_path,
			uint16_t code_line,
			char *inputStream)
{

	if(!s_isInitialized)
	{
		log_init();
		if(!s_isInitialized)
			return;
	}


	char time_buff[LENGTH_TIME_BUFF];
	get_time_string(time_buff);
	get_time_string(time_buff);

	char file_name_buf[LENGTH_FILE_NAME_BUFF];
	memset(file_name_buf, 0, LENGTH_FILE_NAME_BUFF);
	GetFileName(file_path, file_name_buf);

	char log_buff[LENGTH_LOG_STRING_BUFF];
	snprintf(log_buff, LENGTH_LOG_STRING_BUFF, "[%s][%s]", log_title[severity], time_buff);
	snprintf(log_buff, LENGTH_LOG_STRING_BUFF,  "%s[%s][%d]", log_buff, file_name_buf, code_line);
	snprintf(log_buff, LENGTH_LOG_STRING_BUFF,  "%s[%s]\r\n", log_buff, inputStream);

	open_log_file();

	uint32_t bw=0;
	if( FR_OK == s_res)
		s_res = f_write(s_pfile, log_buff, strlen(log_buff), &bw);

	//printf("Writed data len:%d\r\n", bw);
//	if(s_res != FR_OK)
//		printf("F_write FAILED and return value %d\r\n", s_res);

	close_log_file();

	printf("%s\r\n", log_buff);
}
