/*
 * common.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: siwei
 */

#include "common.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string>
#include <sys/stat.h>

extern int level;

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
pipe::pipe(std::string fifoName)
{
    fifo = fifoName;
    fd = open(fifoName.c_str(), O_RDWR);
    if (fd < 0)
    {
        ERROR("open fd error,errno is %s,errno is %d", strerror(errno), errno);
        if (errno == 2)
        {
            ERROR("%s is not existed,created it", fifoName.c_str());
            int ret = mkfifo(fifoName.c_str(), S_IFIFO | 0666);
            if (ret == -1)
            {
                ERROR("mkfifo return -1,%s created failed", fifoName.c_str());
            } else
            {
                DEBUG("mkfifo %s successed", fifoName.c_str());
                fd = open(fifoName.c_str(), O_RDWR);
                owner = 1;
            }
        }
    }
}

pipe::~pipe()
{
    close(fd);
    if (owner == 1)
    {
        DEBUG("%s created by this,unlink it", fifo.c_str());
        unlink(fifo.c_str());
    }
    DEBUG("%s pipe end success", fifo.c_str());
}

int pipe::senMsg(char* buf,int length)
{
    int len = write(fd, buf, length);
    if (len < 0)
    {
        ERROR("write error,errno is %s", strerror(errno));
        return -1;
    }
    return 0;
}

int pipe::recvMsg(char* buff,int len)
{
    return read(fd, buff, len);
}

int pipe::getFd()
{
    return fd;
}
}

