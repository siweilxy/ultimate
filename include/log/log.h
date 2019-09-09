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
int log_init();

#define LOG(format, p,...)  f(format, p,  ##__VA_ARGS__)

void f(std::string p,...)
{
    char buf[256] = {0};
    va_list list;
    va_start(list, p);
    vsprintf(buf, p.c_str(), list);
    printf("%s", buf);
    va_end(list);

}
void g(char* str)
{
    printf("%s\n",str);
}

#endif /* INCLUDE_LOG_H_ */
