/*
 * log.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: siwei
 */

#include "log.h"
#include <stdio.h>

int level =0;

int log_init()
{
    printf("日志模块初始化成功\n");
    return 0;
}

int setLog(char* msg)
{
    if (strcmp(msg, "DEBUG") == 0)
    {
        level = 0;
        DEBUG("DEBUG");
    } else if (strcmp(msg, "WARN") == 0)
    {
        level = 2;
        WARN("WARN");
    } else if (strcmp(msg, "ERROR") == 0)
    {
        level = 3;
        ERROR("ERROR");
    } else if (strcmp(msg, "INFO") == 0)
    {
        level = 1;
        INFO("INFO");
    } else
    {
        ERROR("错误消息:%s", msg);
        return -1;
    }
    return 0;
}

