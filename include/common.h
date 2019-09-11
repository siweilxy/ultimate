/*
 * common.h
 *
 *  Created on: Sep 11, 2019
 *      Author: siwei
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <stdio.h>
#include "log.h"
#include <stdlib.h>
#include "json.hpp"
#include <iostream>
#include <fstream>

int getfile(const std::string& path, std::string& result)
{
    char buffer[256];
    std::ifstream in(path);
    if (!in.is_open())
    {
        ERROR("Error opening file");
        return -1;
    }
    while (!in.eof())
    {
        in.getline(buffer, 10000);
        result = result + buffer;
    }
    DEBUG("result is %s",result.c_str());
    return 0;
}



#endif /* INCLUDE_COMMON_H_ */
