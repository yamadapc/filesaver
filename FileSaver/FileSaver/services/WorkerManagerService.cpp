//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#include "WorkerManagerService.h"
#include "factory/FileSaverFactory.h"
#include <thread>

filesaver::FileSaver& FileSaverService::getInstance ()
{
    static filesaver::FileSaverFactory instance;
    return instance.getRef ();
}

void FileSaverService::start ()
{
    auto& instance = getInstance ();
    instance.start ();
}

void FileSaverService::destroy ()
{
    auto& workerManager = getInstance ();
    workerManager.stop ();
}
