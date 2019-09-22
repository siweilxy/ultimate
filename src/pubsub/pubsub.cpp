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
#include <sys/socket.h>
#include "pubsub/pubsub.h"

static void* runRecvThread(void* args)
{
    pubsub::getInstance()->recvMsg();
    return nullptr;
}

static void* runHeatBeatThread(void* args)
{
    pubsub::getInstance()->heartBeatMsg();
    return nullptr;
}

static void* runSendThread(void* args)
{
    pubsub::getInstance()->sendMsg();
    return nullptr;
}

int init()
{
    auto ps = pubsub::getInstance();
    return ps->init();
}

int pub()
{
    auto ps = pubsub::getInstance();
    return ps->pub();;
}

int sub()
{
    auto ps = pubsub::getInstance();
    return ps->sub();
}

int pubsub::initLog()
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
    if (plevel == nullptr)
    {
        level = 0;
        printf("LOG_LEVEL 没有设置,使用默认设置INFO");
    } else
    {
        level = atoi(plevel);
    }

    google::InitGoogleLogging(name);
    FLAGS_stderrthreshold = level;
    FLAGS_logbufsecs =0; //立即写入
    std::string info_log = name;
    google::SetLogDestination(google::INFO,
            (info_log + "PUBSUB_INFO_").c_str());
    google::SetLogDestination(google::WARNING,
            (info_log + "PUBSUB_WARNING_").c_str());
    google::SetLogDestination(google::ERROR,
            (info_log + "PUBSUB_ERROR_").c_str());
    google::SetLogDestination(google::FATAL,
            (info_log + "PUBUSB_FATAL_").c_str());

    LOG(INFO)<< "INFO log path is "<<(info_log + "PUBSUB_INFO");
    LOG(WARNING)<< "WARNING log path is "<<(info_log + "PUBSUB_WARNING");
    LOG(ERROR)<< "ERROR log path is "<<(info_log + "PUBSUB_ERROR");

    return 0;
}

int pubsub::initDevice()
{
    int ret = 0;
    udpFd = socket(AF_INET,SOCK_DGRAM,0);
    memset(&servaddr,0,sizeof(servaddr));
    memset(&cliaddr,0,sizeof(cliaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    while(udpFd < 60999)
    {
        servaddr.sin_port=udpPort;
        ret = bind(udpFd,(struct sockaddr *)&servaddr,sizeof(servaddr));
        if(ret != 0)
        {
            LOG(ERROR)<<"bind error,errno is "<<errno<<" :"<<strerror(errno);
            if(errno == 98)
            {
                LOG(ERROR)<<"rebind  udpPort";
                udpPort ++;
                continue;
            }else
            {
                LOG(FATAL)<<"bind error,errno is "<<errno<<" :"<<strerror(errno);
                break;
            }
        }else
        {
            LOG(WARNING)<<"bind success ,port is "<<udpPort;
            break;
        }
    }

    return ret;
}

int pubsub::initRecvThread()
{
    pthread_create(&recvThread,nullptr,runRecvThread,nullptr);
    LOG(INFO)<<"recvThread init success";
    return 0;
}

int pubsub::initHeartBeatThread()
{
    pthread_create(&heartBeatThread,nullptr,runHeatBeatThread,nullptr);
    LOG(INFO)<<"heartBeatThread init success";
    return 0;
}

int pubsub::initSendMsgThread()
{
    pthread_create(&sendThread,nullptr,runSendThread,nullptr);
    LOG(INFO)<<"sendThread init success";
    return 0;
}

int pubsub::init()
{
    int ret = initLog();
    if(ret != 0)
    {
        printf("init log error");
        return -1;
    }

    ret = initDevice();
    if(ret != 0)
    {
        LOG(FATAL)<<"initDevice ERROR";
        return -1;
    }

    ret = initRecvThread();
    if(ret != 0 )
    {
        LOG(FATAL)<<"initRecvThread ERROR";
        return -1;
    }

    ret = initHeartBeatThread();
    if(ret != 0 )
    {
        LOG(FATAL)<<"initHeartBeatThread ERROR";
        return -1;
    }

    ret = initSendMsgThread();
    if(ret != 0)
    {
        LOG(FATAL)<<"initSendMsgThread ERROR";
        return -1;
    }

    return 0;
}

int pubsub::pub()
{
    return 0;
}

int pubsub::sub()
{
    return 0;
}

pubsub::pubsub()
{
    udpPort = 10086;
}

pubsub::~pubsub()
{
    pthread_join(recvThread,nullptr);
    pthread_join(heartBeatThread,nullptr);
    pthread_join(sendThread,nullptr);
    delete instance;
}

pubsub* pubsub::getInstance()
{
    return instance;
}

int pubsub::sendMsg()
{
    socklen_t len = 0;

    PUBSUB::pubsub_msg_t * msg;
    std::vector<PUBSUB::pubsub_msg_t> msgForSend;
    while(1)
    {
        pthread_mutex_lock(&sendMutex);
        while(sendMsgs.empty())
        pthread_cond_wait(&sendCond,&sendMutex);
        msgForSend = std::move(sendMsgs);
        pthread_mutex_unlock(&sendMutex);
        for(auto msg:msgForSend)
        {
            LOG(INFO)<<msg.type;
        }
    }
    return 0;
}

int pubsub::heartBeatMsg()
{
    PUBSUB::pubsub_msg_t heatBeat;
    heatBeat.length=0;
    heatBeat.magic=10205794;
    heatBeat.type =PUBSUB::HEATBEAT;
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&sendMutex);
        sendMsgs.push_back(heatBeat);
        pthread_mutex_unlock(&sendMutex);
        pthread_cond_signal(&sendCond);
    }
    return 0;
}

int pubsub::recvMsg()
{
    socklen_t len = 0;
    PUBSUB::pubsub_msg_t * msg;
    while(1)
    {
        pthread_mutex_lock(&recvMutex);
        recvfrom(udpFd,recvBuff,sizeof(recvBuff),0,(struct sockaddr *)&cliaddr,&len);
        msg = (PUBSUB::pubsub_msg_t *)recvBuff;
        recvMsgs.push_back(*msg);
        pthread_mutex_unlock(&recvMutex);
        pthread_cond_signal(&recvCond);
    }
    return 0;
}
