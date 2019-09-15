/*
 * bus.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: siwei
 */

#include "worker.h"
#include "common.h"

extern int level;

void bus()
{
    while (1)
    {
        sleep(1);
        DEBUG("bus 运行中");
        //break;
    }
}

int main()
{
    DEBUG("bus启动");
    processFunc f = bus;
    worker w(f,f,f);

    return 0;
}
