/*
 * pubTest.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: siwei
 */

#include "pubsub/pubsub.h"
#include <stdio.h>

int main()
{
    int ret = init();
    if(ret != 0)
    {
        printf("init error");
    }

    pub();
    while(1){sleep(1);}
    return 0;
}
