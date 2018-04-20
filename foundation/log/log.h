//////////////////////////////////////////////////////////////////////////
/*
��־����
*/
//////////////////////////////////////////////////////////////////////////
#ifndef  DEF_LOG_H_
#define  DEF_LOG_H_

#pragma GCC diagnostic ignored "-Wwrite-strings"

#define LOG_DEBUG  1
#define LOG_ERROR  2
#define LOG_INFO   3
#define LOG_STAT   4
#define LOG_DUMP   5
#define LOG_SQL    6

#define LOGGING(log_type, format, ...) logit(log_type, __FILE__, __LINE__, format, ##__VA_ARGS__)

void logit(int priority, char *filename, int line, const char *format, ...);
void logit_large(int priority, char *filename, int line, const char *hit, char *msg);

#pragma(pop)

#endif
