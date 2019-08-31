//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include <chrono>
#include <iostream>

#include "Worker.h"

Worker::Worker(int id, WorkQueue<std::string> &queue,
               WorkQueue<std::shared_ptr<filesize_service::FileEntry>> &rqueue)
    : id(id), workQueue(queue), resultQueue(rqueue) {}

void Worker::start() {
  running = true;
  while (running) {
    auto maybeFile = workQueue.frontWithTimeout(std::chrono::milliseconds(10));
    if (maybeFile) {
      auto file = maybeFile.value();
      auto fileEntry = filesize_service::FileEntry::fromPath(std::move(file));
      filesProcessed += 1;
      resultQueue.push(fileEntry);

      for (const auto &child : fileEntry->children()) {
        workQueue.push(child);
      }
    }
  }
}

unsigned long Worker::getFilesProcessed() { return filesProcessed; }

void Worker::stop() { running = false; }
