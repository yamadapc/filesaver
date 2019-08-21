//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKERMANAGER_H
#define FILE_SAVER_WORKERMANAGER_H

#include <thread>
#include <vector>

#include "../services/FileSizeService.h"
#include "./Worker.h"

class WorkerManager {
public:
  WorkQueue<std::shared_ptr<filesize_service::FileEntry>> resultQueue;

  unsigned getNumWorkers();

  void start(unsigned int wantedWorkers);
  void stop();
  void join();
  void scan(std::string filepath);

private:
  WorkQueue<std::string> fileWorkQueue;

  std::vector<std::shared_ptr<Worker>> workers;
  std::vector<std::thread> workerThreads;
};

#endif // FILE_SAVER_WORKERMANAGER_H
