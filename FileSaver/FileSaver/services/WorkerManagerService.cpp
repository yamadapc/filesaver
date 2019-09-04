//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#include <thread>
#include "WorkerManagerService.h"

filesaver::FileSaver &FileSaverService::getInstance() {
    static filesaver::FileSaver instance("/tmp/filesaver.db");
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
