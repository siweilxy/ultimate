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

int log_init();

#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"

#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K"


#define DEBUG(format, ...)                  \
{                                               \
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf("[DEBUG]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n",getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__);                            \
}

#define WARN(format, ...)                  \
{                                               \
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf(YELLOW "[WARN]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n" NONE,getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__);                            \
}

#define ERROR(format, ...)                  \
{                                               \
    time_t t = time(NULL);          \
    struct tm ttt = *localtime(&t);\
    printf(RED "[ERROR]" "[" __FILE__ "][%d:%4d-%02d-%02d %02d:%02d:%02d][%s:%d] " format "\n" NONE,getpid(),                     \
            ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday, ttt.tm_hour,        \
            ttt.tm_min, ttt.tm_sec, __FUNCTION__ ,  __LINE__, ##__VA_ARGS__);                            \
}


#endif /* INCLUDE_LOG_H_ */
