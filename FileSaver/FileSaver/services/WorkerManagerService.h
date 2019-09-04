//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#ifndef FILESAVER_WORKERMANAGERSERVICE_H
#define FILESAVER_WORKERMANAGERSERVICE_H

#include "FileSaver.h"

class FileSaverService {
public:
    static filesaver::FileSaver& getInstance();
    static void start();
    static void destroy();

private:
    FileSaverService() = default;
};

#endif //FILESAVER_WORKERMANAGERSERVICE_H
