#include <iostream>
#include "log.h"

int main()
{
    log_init();
    f("%s%s", "good", "boy");
    LOG("%s%s%d%d","good","boy",1,2);

    return 0;
}
