/*
 * slave.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: siwei
 */

#include "log.h"
extern int level;
int main()
{
    DEBUG("slave启动");
    while (1)
    {
        sleep(1);
        //break;
    }
    return 0;
}
