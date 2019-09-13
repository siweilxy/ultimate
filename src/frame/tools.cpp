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

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        DEBUG("argc[%d] : %s", i, argv[i]);
    }

    int fd = 0;
    char buf[512] =
    { 0 };
    int len = 0;
    snprintf(buf, sizeof(buf), "%s", argv[1]);
    U::pipe p(FIFO_NAME);

    p.senMsg(buf);

    close(fd);
}
