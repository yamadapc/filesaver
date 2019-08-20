//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKERMANAGER_H
#define FILE_SAVER_WORKERMANAGER_H

#include <thread>
#include <vector>

class WorkQueue {

};

class Worker {
public:
  int id;

  Worker(int id, WorkQueue queue);

  void start();
};

class WorkerManager {
public:
  unsigned getNumWorkers();
  void start();

private:
  std::vector<std::thread> workerThreads;
};

#endif // FILE_SAVER_WORKERMANAGER_H
