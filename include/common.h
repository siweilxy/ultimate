/*
 * common.h
 *
 *  Created on: Sep 11, 2019
 *      Author: siwei
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_
#include <stdio.h>
#include "log/log.h"
#include <stdlib.h>
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <netinet/in.h>

#define FIFO_NAME "entry"

typedef struct fd_s
{
    int fd;
    std::string file;
} fd_t;

enum msgType
{
    ul_log = 0, ul_end, ul_process_success, ul_other
};

typedef struct internal_msg_s
{
    char srcName[512];
    char destName[512];
    char msg[512];
    char reslut[512];
    char masterIp[512];
    msgType type;
} internal_msg_t;

namespace PUBSUB
{
enum pubsubMsgType
{
    HEATBEAT2SERVER = 0,
    HEATBEAT2CLENT
};

//typedef struct pubusb_msg_s
//{
//    char remoteIP[255];
//    int length;
//    int type;
//    int magic;
//    char msg[0];
//} pubsub_msg_t;

typedef struct session_s
{
    int uid;
    int port;
    std::string remoteIp;
    sockaddr_in remote;
} session_t;
}

int getfile(const std::string& path, std::string& result);

typedef void (*initFunc)();

typedef void (*doneFunc)();

typedef void (*processFunc)();

namespace U
{
class pipe
{
private:
    int fd = 0;
    std::string fifo;
    int owner = 0;
public:
    pipe(std::string fifoName);
    ~pipe();
    int senMsg(char* buf, int length);
    int recvMsg(char* buff, int len);
    int getFd();
};
}
#endif /* INCLUDE_COMMON_H_ */
