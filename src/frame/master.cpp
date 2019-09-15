/*
 * master.cpp
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
#include <pthread.h>
#include <signal.h>
#include "json.hpp"
#include "common.h"
#include "log.h"
using json = nlohmann::json;

typedef struct fd_s
{
    int fd;
    std::string file;
} fd_t;

std::map<pid_t, fd_t> pids;
int fd = 0;

char* cfgPath = nullptr;
char* binPath = nullptr;
pthread_t scanThread_t;
static pthread_mutex_t scanMutex = PTHREAD_MUTEX_INITIALIZER;
void * scanThread(void* param)
{
    int status = 0;
    while(1)
    {
        sleep(4);
        pthread_mutex_lock(&scanMutex);
        for(auto pid:pids)
        {
            int ret = kill(pid.first,0);
            if(ret ==-1)
            {
                ERROR("kill %d ret is %d,errno is %d:%s",pid.first,ret,errno,strerror(errno));
                pids.erase(pid.first);
            }else
            {
                ret = waitpid(pid.first,&status,WNOHANG);
                DEBUG("file is %s pid is %d,ret is %d,status is %d",pid.second.file.c_str(), pid.first,ret,status);
            }
        }
        pthread_mutex_unlock(&scanMutex);
    }
    return nullptr;
}

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
        exit(0);
    }
    return 0;
}

int runWorkers(json j)
{
    auto workers = j["worker"];
    auto number = j["workerNo"];
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
            exit(0);
        } else
        {
            pids[pid]={0,file};
        }
    }

    pthread_create(&scanThread_t,nullptr,scanThread,nullptr);

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

void cleanSource()
{
    DEBUG("%s 开始",__FUNCTION__);
    int status = 0;
    pthread_mutex_lock(&scanMutex);
    for(auto pid:pids)
    {
        int ret = kill(pid.first,9);
        if(ret ==-1)
        {
            ERROR("kill %d ret is %d,errno is %d:%s",pid.first,ret,errno,strerror(errno));
        }else
        {

            ret = waitpid(pid.first,&status,0);
            DEBUG("file is %s pid is %d,ret is %d,status is %d",pid.second.file.c_str(), pid.first,ret,status);
        }
    }
    pthread_mutex_unlock(&scanMutex);

    close(fd);
    unlink(FIFO_NAME); //删除管道文件
    DEBUG("%s 结束",__FUNCTION__);
}

int main()
{
    std::string result;
    int ret = 0;
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
                    if(strcmp(buf,"END") == 0)
                    {
                        button = 1;
                        ERROR("程序退出");
                        break;
                    }else if (strcmp(buf,"DEBUG") == 0)
                    {
                        level = 0;
                        DEBUG("DEBUG");
                        break;
                    }else if(strcmp(buf,"WARN") == 0)
                    {
                        level = 2;
                        WARN("WARN");
                        break;
                    }else if(strcmp(buf,"ERROR") == 0)
                    {
                        level = 3;
                        ERROR("ERROR");
                        break;
                    }else if(strcmp(buf,"INFO") == 0)
                    {
                        level = 1;
                        INFO("INFO");
                        break;
                    }
                    else
                    {
                        ERROR("错误消息");
                    }
                }
            }
        }
    }

    cleanSource();
    return 0;
}
