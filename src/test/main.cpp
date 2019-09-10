#include <iostream>
#include "log.h"

int main()
{
    log_init();

    DEBUG("%s%s%d%d", "good", "boy", 1, 2);
    ERROR("%s", "error");
    WARN("%s", "warn");

    return 0;
}
