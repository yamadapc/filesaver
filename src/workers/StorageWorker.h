//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#ifndef FILE_SAVER_STORAGEWORKER_H
#define FILE_SAVER_STORAGEWORKER_H

#include "../data/WorkQueue.h"
#include "../services/StorageService.h"

namespace filesaver {

class StorageWorker {
public:
  StorageWorker(int id, const StorageService &storageService,
                WorkQueue<std::shared_ptr<FileEntry>> &inputQueue);

private:
  bool running = false;

  int id;
  StorageService storageService;
  WorkQueue<std::shared_ptr<FileEntry>> &inputQueue;
};

} // namespace filesaver

#endif // FILE_SAVER_STORAGEWORKER_H
