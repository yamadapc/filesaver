//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <thread>

#include "FileSaver.h"

FileSaver::FileSaver() = default;

int FileSaver::main(int argc, char *argv[]) {
  std::cout << "file-saver" << std::endl;
  auto numCpus = std::thread::hardware_concurrency();
  std::cout << "Going to run on " << argc - 1 << " files and " << numCpus
            << " threads" << std::endl;

  for (int i = 1; i < argc; i++) {
    auto filename = argv[i];
    std::cout << "- " << filename << std::endl;
  }
}
