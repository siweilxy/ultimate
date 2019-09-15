/*
 * tools.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: siwei
 */

#include "common.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

extern int level;

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        DEBUG("argc[%d] : %s", i, argv[i]);
    }

    internal_msg_t msg;
    msg.type = (msgType)atoi(argv[1]);
    snprintf(msg.destName,sizeof(msg.destName),"%s",argv[2]);
    snprintf(msg.msg,sizeof(msg.msg),"%s",argv[3]);
    snprintf(msg.srcName,sizeof(msg.srcName),"%s","tools");

    int fd = 0;
    char buf[10240] =
    { 0 };
    int len = 0;
    memcpy(buf, &msg,sizeof(msg));
    U::pipe p(FIFO_NAME);

    p.senMsg(buf,sizeof(buf));

    close(fd);
}
