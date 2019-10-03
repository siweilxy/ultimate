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
#include <arpa/inet.h>
#include "pubsub/pubsub.h"

static void* runRecvThread(void* args)
{
    pubsub::getInstance()->recvMsg();
    return nullptr;
}

static void* runRecvFromServerThread(void* args)
{
    pubsub::getInstance()->recvMsgFromServer();
    return nullptr;
}

static void* runSolveMsgThread(void* args)
{
    pubsub::getInstance()->solveMsg();
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
    FLAGS_logbufsecs = 0; //立即写入
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
    udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    while (udpFd < 60999)
    {
        servaddr.sin_port = udpPort;
        ret = bind(udpFd, (struct sockaddr *) &servaddr, sizeof(servaddr));
        if (ret != 0)
        {
            LOG(ERROR)<<"bind error,errno is "<<errno<<" :"<<strerror(errno);
            if(errno == 98)
            {
                LOG(ERROR)<<"rebind  udpPort";
                udpPort ++;
                continue;
            } else
            {
                LOG(FATAL)<<"bind error,errno is "<<errno<<" :"<<strerror(errno);
                break;
            }
        } else
        {
            LOG(WARNING)<<"bind success ,port is "<<udpPort;
            break;
        }
    }

    ret = initTcpClient();
    if (ret != 0)
    {
        LOG(FATAL)<<"initTcpClient error";
    }

    return ret;
}

int pubsub::initTcpClient()
{
    tcpFd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serveradd_tcp, 0, sizeof(serveradd_tcp));
    serveradd_tcp.sin_family = AF_INET;
    serveradd_tcp.sin_port = htons(12345);
    char ip[255] = "192.168.31.238";
    char recvBuf[255] =
    { 0 };
    serveradd_tcp.sin_addr.s_addr = inet_addr(ip);
    if (connect(tcpFd, (struct sockaddr*) &serveradd_tcp, sizeof(serveradd_tcp))
            < 0)
    {
        LOG(FATAL)<<"connect error,errno is "<<errno<<" :"<<strerror(errno);
        return -1;
    }

//    auto len = write(tcpFd,&ip[0],strlen(ip));
//    LOG(INFO)<<"write len is "<<len;
//    len = read(tcpFd,recvBuf,sizeof(recvBuf));
//    LOG(INFO)<<"msg is "<<recvBuf;
    return 0;
}

int pubsub::initRecvThread()
{
    pthread_create(&recvThread, nullptr, runRecvThread, nullptr);
    LOG(INFO)<<"recvThread init success";
    return 0;
}

int pubsub::initHeartBeatThread()
{
    pthread_create(&heartBeatThread, nullptr, runHeatBeatThread, nullptr);
    LOG(INFO)<<"heartBeatThread init success";
    return 0;
}

int pubsub::initSendMsgThread()
{
    pthread_create(&sendThread, nullptr, runSendThread, nullptr);
    LOG(INFO)<<"runSendThread init success";
    return 0;
}

int pubsub::initRecvFromServerThread()
{
    pthread_create(&recvFromServerThread, nullptr, runRecvFromServerThread,
            nullptr);
    LOG(INFO)<<"sendThread init success";
    return 0;
}

int pubsub::initSolveMsgThread()
{
    pthread_create(&solveThread, nullptr, runSolveMsgThread,
            nullptr);
    LOG(INFO)<<"solveThread init success";
    return 0;
}

int pubsub::init()
{
    int ret = initLog();
    if (ret != 0)
    {
        printf("init log error");
        return -1;
    }

    ret = initDevice();
    if (ret != 0)
    {
        LOG(FATAL)<<"initDevice ERROR";
        return -1;
    }

    ret = initRecvThread();
    if (ret != 0)
    {
        LOG(FATAL)<<"initRecvThread ERROR";
        return -1;
    }

    ret = initHeartBeatThread();
    if (ret != 0)
    {
        LOG(FATAL)<<"initHeartBeatThread ERROR";
        return -1;
    }

    ret = initSendMsgThread();
    if (ret != 0)
    {
        LOG(FATAL)<<"initSendMsgThread ERROR";
        return -1;
    }

    ret = initRecvFromServerThread();
    if (ret != 0)
    {
        LOG(FATAL)<<"initRecvFromServerThread ERROR";
        return -1;
    }

    ret = initSolveMsgThread();
    if (ret != 0)
    {
        LOG(FATAL)<<"initSolveMsgThread ERROR";
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
    pthread_join(recvThread, nullptr);
    pthread_join(heartBeatThread, nullptr);
    pthread_join(sendThread, nullptr);
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
    while (1)
    {
        pthread_mutex_lock(&sendMutex);
        while (sendMsgs.empty())
            pthread_cond_wait(&sendCond, &sendMutex);
        msgForSend = std::move(sendMsgs);
        pthread_mutex_unlock(&sendMutex);
        for (auto msg : msgForSend)
        {
            switch (msg.type)
            {
            case PUBSUB::HEATBEAT2SERVER:
            {
                auto len = write(tcpFd, &msg, sizeof(msg));
                LOG(INFO)<<"write len is "<<len;
                break;
            }
            case PUBSUB::HEATBEAT2CLENT:
            {
                LOG(INFO)<<"HEATBEAT2CLENT:"<<msg.type;
                break;
            }default:
            {
                LOG(ERROR)<<"type error :"<<msg.type;
            }
        }
    }
}
    return 0;
}

int pubsub::heartBeatMsg()
{
    char remoteIp[255] = "12345";
    PUBSUB::pubsub_msg_t heatBeat2Server;
    PUBSUB::pubsub_msg_t heatBeat2Client;
    heatBeat2Server.length = sizeof(PUBSUB::pubsub_msg_t);
    heatBeat2Server.magic = 10205794;
    heatBeat2Server.type = PUBSUB::HEATBEAT2SERVER;
    snprintf(heatBeat2Server.remoteIP,sizeof(heatBeat2Server.remoteIP),remoteIp,strlen(remoteIp));
    heatBeat2Client.length = sizeof(PUBSUB::pubsub_msg_t);;
    heatBeat2Client.magic = 10205794;
    heatBeat2Client.type = PUBSUB::HEATBEAT2CLENT;
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&sendMutex);
        sendMsgs.push_back(heatBeat2Server);
        sendMsgs.push_back(heatBeat2Client);
        pthread_mutex_unlock(&sendMutex);
        pthread_cond_signal(&sendCond);
    }
    return 0;
}

int pubsub::recvMsg()
{
    socklen_t len = 0;
    PUBSUB::pubsub_msg_t * msg;
    while (1)
    {
        recvfrom(udpFd, recvBuff, sizeof(recvBuff), 0,
                (struct sockaddr *) &cliaddr, &len);
        msg = (PUBSUB::pubsub_msg_t *) recvBuff;
        pthread_mutex_lock(&recvMutex);
        recvMsgs.push_back(*msg);
        pthread_mutex_unlock(&recvMutex);
        pthread_cond_signal(&recvCond);
    }
    return 0;
}

int pubsub::recvMsgFromServer()
{
    socklen_t len = 0;
    PUBSUB::pubsub_msg_t * msg;
    int msgs = 0;
    while (1)
    {
        auto len = read(tcpFd, recvFromBuff, sizeof(recvFromBuff));
        if(len == 0)
        {
            LOG(ERROR)<<"len is 0:"<<strerror(errno);
            sleep(3600);
        }
        msgs++;
        msg = (PUBSUB::pubsub_msg_t *) recvFromBuff;
        pthread_mutex_lock(&recvMutex);
        recvMsgs.push_back(*msg);
        LOG(INFO)<<"recv msgs:"<<msgs;
        pthread_mutex_unlock(&recvMutex);
        pthread_cond_signal(&recvCond);
    }
    return 0;
}

int pubsub::solveMsg()
{
    socklen_t len = 0;

    PUBSUB::pubsub_msg_t * msg;
    std::vector<PUBSUB::pubsub_msg_t> msgRecv;
    while (1)
    {
        pthread_mutex_lock(&recvMutex);
        while (recvMsgs.empty())
            pthread_cond_wait(&recvCond, &recvMutex);
        msgRecv = std::move(recvMsgs);
        pthread_mutex_unlock(&recvMutex);
        LOG(INFO)<<"recv msg size:"<<msgRecv.size();
        for (auto msg : msgRecv)
        {
            LOG(INFO)<<"recv msg:"<<msg.magic;
        }
    }
    return 0;
}
