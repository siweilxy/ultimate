/*
 * pubsub.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: siwei
 */

#include "logging.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
int init()
{
    char* name = nullptr;
    char* plevel = nullptr;
    int level = 0;
    name = getenv("PUBSUB_LOG_PATH");
    if (name == nullptr)
    {
        printf("PUBSUB_LOG_PATH 没有设置");
        return -1;
    }

    plevel = getenv("LOG_LEVEL");
    if(plevel == nullptr)
    {
        level = 0;
        printf("LOG_LEVEL 没有设置,使用默认设置INFO");
    }else
    {
        level = atoi(plevel);
    }

    google::InitGoogleLogging(name);
    FLAGS_stderrthreshold = level;
    std::string info_log = name;
    google::SetLogDestination(google::INFO, (info_log+ "PUBSUB_INFO_").c_str());
    google::SetLogDestination(google::WARNING, (info_log+"PUBSUB_WARNING_").c_str());
    google::SetLogDestination(google::ERROR, (info_log+"PUBSUB_ERROR_").c_str());
    google::SetLogDestination(google::FATAL, (info_log+"PUBUSB_FATAL_").c_str());

    LOG(INFO)<< "INFO log path is "<<(info_log + "PUBSUB_INFO");
    LOG(WARNING)<< "WARNING log path is "<<(info_log + "PUBSUB_WARNING");
    LOG(ERROR)<< "ERROR log path is "<<(info_log + "PUBSUB_ERROR");

    return 0;
}

int pub()
{
    return 0;
}

int sub()
{
    return 0;
}

