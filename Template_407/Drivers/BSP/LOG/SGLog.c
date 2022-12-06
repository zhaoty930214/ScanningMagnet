#include "SGLog.h"
#include "ff.h"
#include "rtc.h"
#include "fattester.h"

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


char *log_title[6]=
{
	"INVALID",
    "FATAL",
	"CRITICAL",
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
		printf("Log file exists, open and append\r\n");
	}
}

void close_log_file(void)
{
	f_close(s_pfile);
}

void get_time_string(char *buff)
{
	uint8_t year, month, date, week;
	uint8_t hour, min, sec, ampm=0;
	rtc_get_time(&hour, &min, &sec, &ampm);
	rtc_get_date(&year, &month, &date, &week);

	printf("get_time_string:%04d-%02d-%02d %02d:%02d:%02d\r\n",
			year+2000, month, date, hour, min, sec);

	sprintf(buff,  "%04d-%02d-%02d %02d:%02d:%02d",
			year+2000, month, date, hour, min, sec);
}

void logMessage(
			EventLogSeverity_t severity,
			char *file_name,
			uint16_t code_line,
			char *inputStream)
{
	if(!s_isInitialized)
	{
		log_init();
		if(!s_isInitialized)
			return;
	}

	char log_buff[200];
	char time_buff[30];
	get_time_string(time_buff);
	get_time_string(time_buff);

	sprintf(log_buff,  "[%s][%s]", log_title[severity], time_buff);
	sprintf(log_buff,  "%s[%s][%d]", log_buff, file_name, code_line);
	sprintf(log_buff, "%s[%s]\r\n", log_buff, inputStream);

	open_log_file();

	uint32_t bw=0;
	if( FR_OK == s_res)
		s_res = f_write(s_pfile, log_buff, strlen(log_buff), &bw);

	printf("Writed data len:%d\r\n", bw);
	printf("F_write return value %d\r\n", s_res);

	close_log_file();

	printf("%s\r\n", inputStream);
}
