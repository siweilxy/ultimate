#include <iostream>
#include "log.h"
#include <pthread.h>

extern int level;

void* test(void*)
{
    while (1)
    {
        DEBUG("%s%s%d%d", "good", "boy", 1, 2);
        ERROR("%s", "error");
        WARN("%s", "warn");
    }
}

int main()
{
    log_init();

    pthread_t p;
    pthread_create(&p, NULL, test, NULL);

    while (1)
    {
        DEBUG("%s%s%d%d", "good", "boy", 1, 2);
        ERROR("%s", "error");
        WARN("%s", "warn");
    }
    return 0;
}
