/*
 * worker.h
 *
 *  Created on: Sep 15, 2019
 *      Author: siwei
 */

#ifndef INCLUDE_WORKER_H_
#define INCLUDE_WORKER_H_
#include "common.h"

class worker
{
public:
    worker(processFunc pro, initFunc initFunc, doneFunc doneFunc);
    ~worker();

private:
    initFunc init;
    doneFunc done;
    processFunc process;
};

#endif /* INCLUDE_WORKER_H_ */
