#include <iostream>
#include "log.h"
#include <pthread.h>
//#include "pubsub/pubsub.h"
#include <sys/types.h>

extern int level;

void* test(void*)
{
    while (1)
    {
        //DEBUG("%s%s%d%d", "good", "boy", 1, 2);
       // ERROR("%s", "error");
        //WARN("%s", "warn");
        sleep(1);
        WARN("pthrad_self:%d",pthread_self());

    }
}

int main()
{
    log_init();

    pthread_t p;
    pthread_create(&p, NULL, test, NULL);

    pthread_join(p,0);
//    while (1)
//    {
//        DEBUG("%s%s%d%d", "good", "boy", 1, 2);
//        ERROR("%s", "error");
//        WARN("%s", "warn");
//        break;
//    }
    return 0;
}
