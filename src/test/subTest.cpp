/*
 * subTest.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: siwei
 */

#include "pubsub/pubsub.h"



int main()
{

    int ret = init();
    if(ret != 0)
    {
        printf("init error");
    }
    sub();
    while(1){sleep(1);}

    return 0;
}
