/*
 * log.h
 *
 *  Created on: Sep 10, 2019
 *      Author: siwei
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_

#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include "logDefine.h"

int log_init();

#define DEBUG(format, ...)                  \
{                                               \
    if(level <= 0){\
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf(WHITE "[DEBUG]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n",getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__); }                           \
}

#define INFO(format, ...)                  \
{                                               \
    if(level <= 1){\
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf(BLUE "[INFO]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n" NONE,getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__); }                           \
}

#define WARN(format, ...)                  \
{                                               \
    if(level <= 2){\
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf(YELLOW "[WARN]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n" NONE,getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__); }                           \
}

#define ERROR(format, ...)                  \
{                                               \
    if(level <= 3){\
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf(RED "[ERROR]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n" NONE,getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__);  }                          \
}

int setLog(char* msg);

#endif /* INCLUDE_LOG_H_ */
