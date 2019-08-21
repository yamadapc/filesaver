//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include <iostream>

#include "Worker.h"

Worker::Worker(int id, WorkQueue<std::string> &queue,
               WorkQueue<std::shared_ptr<filesize_service::FileEntry>> &rqueue)
    : id(id), workQueue(queue), resultQueue(rqueue) {}

void Worker::start() {
  running = true;
  while (running) {
    auto file = workQueue.front();
    auto fileEntry = filesize_service::FileEntry::fromPath(file);

    if (fileEntry != nullptr) {
      for (const auto &child : fileEntry->children()) {
        workQueue.push(child);
      }
      resultQueue.push(fileEntry);
    }
  }
}

void Worker::stop() { running = false; }
