/*
 * entry.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: siwei
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <errno.h>
#include "json.hpp"
#include "common.h"
#include "log.h"

using json = nlohmann::json;

int runWorker(std::string file,std::string number)
{

    int ret = execlp(file.c_str(),file.c_str(),"1",NULL);
    if(ret == -1)
    {
        ERROR("execl failed,errno is %s,file is %s",strerror(errno),file.c_str());
    }
    return 0;
}

int runWorkers(json j)
{

    auto workers = j["worker"];
    for(auto worker:workers)
    {
        std::string file = worker["file"];
        std::string number = worker["number"];
        DEBUG("file is %s",file.c_str());
        DEBUG("number is %s",number.c_str());
        runWorker(file,number);
    }
    return 0;
}

int main()
{
    char* cfgPath = nullptr;
    std::string result;
    int ret = 0;
    cfgPath = getenv("cfg_path");
    if (cfgPath == nullptr)
    {
        ERROR("cfgPath is nullptr");
        return 1;
    }
    DEBUG("cfgPath is %s", cfgPath);

    ret = getfile(cfgPath, result);
    if(ret != 0)
    {
        ERROR("read file error");
        return 1;
    }

    DEBUG("getfile success");

    json j = json::parse(result);
    ret = runWorkers(j);


    return 0;
}
