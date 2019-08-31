//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#ifndef FILESAVER_WORKERMANAGERSERVICE_H
#define FILESAVER_WORKERMANAGERSERVICE_H

#include "workers/WorkerManager.h"

class WorkerManagerService {
public:
    static WorkerManager& getInstance();
    static void start();
    static void destroy();

private:
    WorkerManagerService() = default;
};

#endif //FILESAVER_WORKERMANAGERSERVICE_H
