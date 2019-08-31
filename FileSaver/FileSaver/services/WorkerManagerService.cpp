//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#include <thread>
#include "WorkerManagerService.h"

FileSaver &FileSaverService::getInstance() {
    static FileSaver instance;
    return instance;
}

void FileSaverService::start() {
    auto& instance = getInstance();
    instance.start();
}

void FileSaverService::destroy() {
    auto& workerManager = getInstance();
    workerManager.stop();
}
