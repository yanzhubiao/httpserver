#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

extern char g_logfile[256];
#define LOG_FILE   g_logfile
#define SHOW_PID
#define MAX_Z_PACKAGE_SIZE  512

FILE *fp_log = NULL;
pthread_mutex_t  mutLogFile;
const char 	  *gszLogCode[]= { "DEBUG", "ERROR", "INFO", "STAT", "DUMP", "SQL" };
pid_t	  gPID;	
const char *PROGRAM_NAME = "Hydra";

/* 
   д��־�ļ� 
example :  
logit(LOG_ERROR,"PID:%d,LINE:%d ---------------- ",getpid(),__LINE__) 
*/

void logit(int priority, char *filename, int line, const char *format, ...)
{

	static FILE *fp_log = NULL;
	static char szLogFile[2048]={0};

	char szBuf[102400], szDate[30]; 

	time_t t=time(NULL);
	struct tm tm; 


	va_list ap;

	localtime_r(&t,&tm);

	/*	
#ifndef DEBUG
if(priority == LOG_SQL)
return ;
#endif
	 */

#ifdef DEBUG
	fp_log = stderr;
#else

	snprintf(szBuf, sizeof(szBuf), "%s%s.log.%04d-%02d-%02d",
			LOG_FILE,
			PROGRAM_NAME,
			tm.tm_year + 1900, 
			tm.tm_mon + 1, 
			tm.tm_mday);


	if (pthread_mutex_lock(&mutLogFile)) {
		fprintf(stderr, "LINE:%d pthread_mutex_lock() error !", __LINE__); 
	}

	if(fp_log == NULL) {
		if ((fp_log = fopen(szBuf, "a")) == NULL) {
			fprintf(stderr,"can't open log file --> [%s] \n", szBuf);
			fp_log=stderr;
		}	
	}
	else {
		if(strcasecmp(szLogFile, szBuf) != 0) {
			fclose(fp_log);
			if ((fp_log = fopen(szBuf, "a")) == NULL) {
				fprintf(stderr,"can't open log file --> [%s] \n", szBuf);
				fp_log=stderr;
			}	
			strcpy(szLogFile,szBuf);
		}
	}		

	if (pthread_mutex_unlock(&mutLogFile)) {
		fprintf(stderr, "LINE:%d pthread_mutex_unlock() error !", __LINE__); 
	}

#endif

	va_start(ap, format);
	vsnprintf(szBuf,sizeof(szBuf)-1, format, ap);
	va_end(ap);

	bzero(szDate,sizeof(szDate));
	strftime(szDate, sizeof(szDate) - 1, "%Y-%m-%d %H:%M:%S", &tm);

	switch(priority) {



#ifdef SHOW_PID	

		case LOG_DEBUG:
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d] %s\n", szDate, "DEBUG", gPID, line, szBuf);
			fflush(fp_log);
			break;  
		case LOG_ERROR:
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d] %s\n", szDate, "ERROR", gPID, line, szBuf);
			fflush(fp_log);
			break;
		case LOG_INFO:	
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d] %s\n", szDate, "INFO", gPID, line, szBuf);
			fflush(fp_log);
			break;
		case LOG_SQL:	
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d] %s\n", szDate, "SQL", gPID, line, szBuf);
			fflush(fp_log);
			break;		
		case LOG_DUMP:	
			fprintf(fp_log, "%s", szBuf);
			fflush(fp_log);
			break;		
		default :
			fprintf(fp_log, "[%s][PID:%d][LINE:%d] %s\n", szDate, gPID, line, szBuf);
			fflush(fp_log);
			break;
#else				

		case LOG_DEBUG:
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d] %s\n", szDate, "DEBUG", filename, line, szBuf);
			fflush(fp_log);
			break;  
		case LOG_ERROR:
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d] %s\n", szDate, "ERROR", filename, line, szBuf);
			fflush(fp_log);
			break;
		case LOG_INFO:	
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d] %s\n", szDate, "INFO", filename, line, szBuf);
			fflush(fp_log);
			break;
		case LOG_SQL:	
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d] %s\n", szDate, "SQL", filename, line, szBuf);
			fflush(fp_log);
			break;		
		case LOG_DUMP:	
			fprintf(fp_log, "%s", szBuf);
			fflush(fp_log);
			break;		
		default :
			fprintf(fp_log,"[%s][FILE:%s LINE:%d] %s\n", szDate, filename, line, szBuf);
			fflush(fp_log);
			break;
#endif

	}

	return;
}

void logit_large(int priority, char *filename, int line, const char *hit, char *msg)
{
	char szBuf[204800]={0},szDate[30]; 

	time_t t=time(NULL);
	struct tm tm; 

	static char szLogFile[2048]={0};

	//va_list ap;

	localtime_r(&t,&tm);
	snprintf(szBuf, sizeof(szBuf), "%s%s.log.%04d-%02d-%02d",
			LOG_FILE,
			PROGRAM_NAME,tm.tm_year + 1900, 
			tm.tm_mon + 1, 
			tm.tm_mday);

	if (pthread_mutex_lock(&mutLogFile))
		LOGGING(LOG_ERROR, "pthread_mutex_lock() error !"); 

	if(fp_log == NULL) {
		if ((fp_log = fopen(szBuf, "a"))==NULL) {
			fprintf(stderr,"can't open log file --> [%s] \n",szBuf);
			fp_log=stderr;
		}
	}
	else {
		if(strcasecmp(szLogFile,szBuf) != 0) {
			fclose(fp_log);
			if ((fp_log = fopen(szBuf, "a"))==NULL) {
				fprintf(stderr,"can't open log file --> [%s] \n",szBuf);
				fp_log=stderr;
			}
			strcpy(szLogFile,szBuf);
		}
	}

	if (pthread_mutex_unlock(&mutLogFile))
		LOGGING(LOG_ERROR, "pthread_mutex_unlock() error !"); 

	bzero(szDate,sizeof(szDate));
	strftime(szDate, sizeof(szDate) - 1, "%Y-%m-%d %H:%M:%S", &tm);

	switch(priority) {

#ifdef SHOW_PID	

		case LOG_DEBUG:
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d]%s%s\n", szDate, "DEBUG", gPID, line, hit, msg);
			fflush(fp_log);
			break;  
		case LOG_ERROR:
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d]%s%s\n", szDate, "ERROR", gPID, line, hit, msg);
			fflush(fp_log);
			break;
		case LOG_INFO:	
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d]%s%s\n", szDate, "INFO", gPID, line, hit, msg);
			fflush(fp_log);
			break;
		case LOG_SQL:	
			fprintf(fp_log, "[%s][%s][PID:%d][LINE:%d]%s%s\n", szDate, "SQL", gPID, line, hit, msg);
			fflush(fp_log);
			break;		
		case LOG_DUMP:	
			fprintf(fp_log, "%s", szBuf);
			fflush(fp_log);
			break;		
		default :
			fprintf(fp_log, "[%s][PID:%d][LINE:%d]%s\n", szDate, gPID, line, szBuf);
			fflush(fp_log);
			break;
#else				

		case LOG_DEBUG:
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d]%s%s\n", szDate, "DEBUG", filename, line, hit, msg);
			fflush(fp_log);
			break;  
		case LOG_ERROR:
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d]%s%s\n", szDate, "ERROR", filename, line, hit, msg);
			fflush(fp_log);
			break;
		case LOG_INFO:	
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d]%s%s\n", szDate, "INFO", filename, line, hit, msg);
			fflush(fp_log);
			break;
		case LOG_SQL:	
			fprintf(fp_log, "[%s][%s][FILE:%s LINE:%d]%s%s\n", szDate, "SQL", filename, line, hit, msg);
			fflush(fp_log);
			break;		
		case LOG_DUMP:	
			fprintf(fp_log, "%s%s", szBuf);
			fflush(fp_log);
			break;		
		default :
			fprintf(fp_log,"[%s][FILE:%s LINE:%d]%s%s\n", szDate, filename, line, hit, msg);
			fflush(fp_log);
			break;
#endif

	}
}
