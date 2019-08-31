//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#include <thread>
#include "WorkerManagerService.h"

WorkerManager &WorkerManagerService::getInstance() {
    static WorkerManager instance;
    return instance;
}

void WorkerManagerService::start() {
    auto& instance = getInstance();
    instance.start(std::thread::hardware_concurrency() * 2);
}

void WorkerManagerService::destroy() {
    auto& workerManager = getInstance();
    workerManager.stop();
}
