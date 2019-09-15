/*
 * worker.cpp
 *
 *  Created on: Sep 15, 2019
 *      Author: siwei
 */

#include "worker.h"

worker::worker(processFunc pro = nullptr, initFunc initFunc = nullptr,
        doneFunc doneFunc = nullptr) :
        process(pro), init(initFunc), done(doneFunc)
{

    if (init != nullptr)
    {
        init();
    }
    if (process != nullptr)
    {
        process();
    }
}

worker::~worker()
{
    if (done != nullptr)
    {
        done();
    }
}

