/*
 * entry.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: siwei
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <map>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <wait.h>
#include "json.hpp"
#include "common.h"
#include "log.h"
using json = nlohmann::json;

typedef struct fd_s
{
    int fd;
} fd_t;

std::map<pid_t, fd_t> pids;
char* cfgPath = nullptr;
char* binPath = nullptr;

int runWorker(std::string file, std::string number)
{
    //int ret = execlp(file.c_str(), file.c_str(), "1", NULL);
    std::string b = "/";
    auto total_file = binPath + b + file;
    int ret = execl(total_file.c_str(), total_file.c_str(), "1", NULL);
    if (ret == -1)
    {
        ERROR("execl 失败,errno 为 %s,file 为 %s", strerror(errno),
                total_file.c_str());
        exit(-1);
    }
    return 0;
}

int runWorkers(json j)
{
    auto workers = j["worker"];
    auto number = j["workerNo"];
    int status = 0;
    int ret =0;
    pid_t pid = 0;
    for (int i = 0; i < number; i++)
    {
        std::string file = workers[i]["file"];
        std::string number = workers[i]["number"];
        DEBUG("file is %s", file.c_str());
        DEBUG("number is %s", number.c_str());
        pid = fork();
        if (pid == 0)
        {
            runWorker(file, number);
        } else
        {
            pids[pid]=
            {   0};
        }
    }

    for (auto pid : pids)
    {
        ret = waitpid(pid.first,&status,0);
        DEBUG("pid is %d,ret is %d,status is %d", pid.first,ret,status);
    }
    DEBUG("worker 启动成功");
    return 0;
}

int initParams()
{
    cfgPath = getenv("cfg_path");
    if (cfgPath == nullptr)
    {
        ERROR("cfgPath is nullptr");
        return -1;
    }
    DEBUG("cfgPath is %s", cfgPath);

    binPath = getenv("BIN_PATH");
    if (binPath == nullptr)
    {
        ERROR("binPath is nullptr");
        return -1;
    }
    DEBUG("binPath is %s", binPath);

    return 0;
}

int main()
{
    std::string result;
    int ret = 0;
    int fd = 0;
    int len = 0;
    int size = 0;
    int status = 0;
    int button = 0;
    int epfd, event_cnt;
    struct epoll_event* ep_events;
    struct epoll_event event;
    char buf[512]={0};

    ret = initParams();
    if (ret != 0)
    {
        ERROR("initParams failed");
        return -1;
    }

    ret = getfile(cfgPath, result);
    if (ret != 0)
    {
        ERROR("read file error");
        return 1;
    }

    DEBUG("getfile success");

    json j = json::parse(result);
    ret = runWorkers(j);
    DEBUG("启动成功");

//    mkfifo(FIFO_NAME, S_IFIFO | 0666);
//    fd = open(FIFO_NAME, O_RDWR);

    U::pipe p(FIFO_NAME);
    fd = p.getFd();

    epfd = epoll_create(50);
    ep_events = (epoll_event*) malloc(sizeof(struct epoll_event) * 50);
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);

    while (button == 0)
    {
        event_cnt = epoll_wait(epfd, ep_events, 50, -1);
        if (event_cnt == -1)
        {
            printf("epoll_wait error\n");
            break;
        }

        for (int i = 0; i < event_cnt; i++)
        {
            while (1)
            {
                memset(buf, 0, sizeof(buf));
                len = read(ep_events[i].data.fd, buf, 512);
//                len = p.recvMsg(buf,512);
                if (len == 0)
                {
                    printf("len == 0!!!!!!!!!!\n");
                } else if (len < 0)
                {
                    if (errno == EAGAIN)
                    {
                        //printf("read EAGAIN,break\n");
                        break;
                    }
                } else
                {
                    printf("收到消息:%s\n",buf);
                    if(strcmp(buf,"end") == 0)
                    {
                        button = 1;
                        DEBUG("程序退出");
                        break;
                    }
                }
            }
        }
    }

    close(fd);
    unlink(FIFO_NAME); //删除管道文件

    return 0;
}
