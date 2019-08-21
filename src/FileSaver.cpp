//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <thread>

#include "FileSaver.h"
#include "workers/WorkerManager.h"

FileSaver::FileSaver() = default;

int FileSaver::main(int argc, char *argv[]) {
  std::cout << "file-saver" << std::endl;
  auto numCpus = std::thread::hardware_concurrency();

  std::cout << "Starting with " << numCpus << " threads" << std::endl;

  WorkerManager workerManager;
  workerManager.start(numCpus);

  if (argc == 1) {
    workerManager.scan(".");
  } else {
    for (int i = 1; i < argc; i++) {
      auto filename = argv[i];
      std::cout << "- " << filename << std::endl;
      workerManager.scan(filename);
    }
  }

  while (true) {
    auto entry = workerManager.resultQueue.front();
    std::cout << entry->filename << " " << entry->size << std::endl;
  }

  workerManager.join();

  return 0;
}
