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
    int senMsg(std::string msg);
    int recvMsg(char* buff,int len);
    int getFd();
};
}
#endif /* INCLUDE_COMMON_H_ */
