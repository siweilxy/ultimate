/*
 * common.h
 *
 *  Created on: Sep 11, 2019
 *      Author: siwei
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <stdio.h>
#include "log.h"
#include <stdlib.h>
#include "json.hpp"
#include <iostream>
#include <fstream>

#define FIFO_NAME "entry"

typedef struct fd_s
{
    int fd;
    std::string file;
} fd_t;

enum msgType
{
  ul_log = 0,
  ul_end,
  ul_other
};

typedef struct internal_msg_s
{
    char srcName[512];
    char destName[512];
    char msg[512];
    char reslut[512];
    char masterIp[512];
    msgType type;
}internal_msg_t;

int getfile(const std::string& path, std::string& result)
{
    char buffer[256];
    std::ifstream in(path);
    if (!in.is_open())
    {
        ERROR("Error opening file");
        return -1;
    }
    while (!in.eof())
    {
        in.getline(buffer, 10000);
        result = result + buffer;
    }
    DEBUG("result is %s", result.c_str());
    return 0;
}

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
    int senMsg(char* buf,int length);
    int recvMsg(char* buff,int len);
    int getFd();
};
}
#endif /* INCLUDE_COMMON_H_ */
