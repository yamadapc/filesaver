//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#include "StorageWorker.h"

filesaver::StorageWorker::StorageWorker(
    int id, const filesaver::StorageService &storageService,
    filesaver::WorkQueue<std::shared_ptr<filesaver::FileEntry>> &inputQueue)
    : id(id), storageService(storageService), inputQueue(inputQueue) {}
