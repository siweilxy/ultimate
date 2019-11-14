/*
 * pubsub.h
 *
 *  Created on: Sep 21, 2019
 *      Author: siwei
 */

#ifndef INCLUDE_PUBSUB_PUBSUB_H_
#define INCLUDE_PUBSUB_PUBSUB_H_

#include <memory>
#include <vector>
#include <netinet/in.h>
#include <pthread.h>
#include "common.h"
#include "pubsub_msg.pb.h"

int init();
int pub();
int sub();

class pubsub
{
public:
    int pub();
    int sub();
    int init();
    int recvMsg();
    int recvMsgFromServer();
    int heartBeatMsg();
    int sendMsg();
    int solveMsg();
    ~pubsub();
    static pubsub* getInstance();
    int initTcpClient();

private:
    pthread_mutex_t recvMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t recvCond = PTHREAD_COND_INITIALIZER;

//    pthread_mutex_t recvFromServerMutex = PTHREAD_MUTEX_INITIALIZER;
//    pthread_cond_t recvFromServerCond = PTHREAD_COND_INITIALIZER;

    pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t sendCond = PTHREAD_COND_INITIALIZER;

    static pubsub* instance;
    pubsub();
    int initDevice();
    int initLog();
    int sendMsg(std::string msg);
    int initRecvThread();
    int initHeartBeatThread();
    int initSendMsgThread();
    int initRecvFromServerThread();
    int initSolveMsgThread();

    std::vector<pubsub_msg_t> recvMsgs;
    std::vector<pubsub_msg_t> sendMsgs;
    std::map<PUBSUB::session_t,std::vector<std::string>> sessions;
    char recvBuff[512];
    char recvFromBuff[512];
    char sendBuff[512];
    int udpFd;
    int tcpFd;
    int udpPort;
    struct sockaddr_in servaddr,cliaddr;
    struct sockaddr_in serveradd_tcp;
    pthread_t recvThread;
    pthread_t heartBeatThread;
    pthread_t sendThread;
    pthread_t recvFromServerThread;
    pthread_t solveThread;

};

pubsub* pubsub::instance = new pubsub();

#endif /* INCLUDE_PUBSUB_PUBSUB_H_ */
