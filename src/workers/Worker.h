//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKER_H
#define FILE_SAVER_WORKER_H

#include <string>

#include "../data/WorkQueue.h"
#include "../services/FileSizeService.h"

class Worker {
public:
  Worker(int id, WorkQueue<std::string> &queue,
         WorkQueue<std::shared_ptr<filesize_service::FileEntry>> &rqueue);

  void start();
  void stop();

private:
  int id;
  bool running = false;

  WorkQueue<std::shared_ptr<filesize_service::FileEntry>> &resultQueue;
  WorkQueue<std::string> &workQueue;
};

#endif // FILE_SAVER_WORKER_H
