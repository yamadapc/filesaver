//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "CommandLineApp.h"

namespace filesaver {

namespace po = boost::program_options;

int CommandLineApp::main(int argc, char **argv) {
  po::options_description publicDescription("filesaver");
  po::variables_map variablesMap;

  publicDescription.add_options()("help", "print this help message")(
      "no-storage", "don't store any data");

  po::store(po::parse_command_line(argc, argv, publicDescription),
            variablesMap);
  po::notify(variablesMap);

  if (variablesMap.count("help")) {
    std::cout << publicDescription << std::endl;
    return 0;
  }

  FileSaver fileSaver;

  if (!variablesMap.count("no-storage")) {
    fileSaver.setupDefaultStorage();
  }

  auto startTime = std::chrono::steady_clock::now();
  fileSaver.start();

  if (argc == 1) {
    fileSaver.scan(".");
  } else {
    for (int i = 1; i < argc; i++) {
      auto filename = argv[i];
      fileSaver.scan(filename);
    }
  }

  while (!fileSaver.areAllTargetsFinished()) {
    long long int milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - startTime)
            .count();

    auto totalFiles = fileSaver.getTotalFiles();
    double filesPerSecond =
        milliseconds > 0 ? 1000.0 * ((double)totalFiles / (double)milliseconds)
                         : 0.0;

    std::cout << "\rWorking... "
              << prettyPrintBytes(fileSaver.getCurrentSizeAt(
                     fileSaver.getTargets()[0].string()))
              << " - " << totalFiles << " files scanned"
              << " - " << filesPerSecond << "/second"
              << " - " << milliseconds << "ms ellapsed"
              << " - " << fileSaver.getStorageQueueSize()
              << " entries waiting to be stored"
              << " - " << fileSaver.getInMemoryEntryCount()
              << " entries held in memory"
              << "                                   ";
    std::cout.flush();

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }

  for (auto &target : fileSaver.getTargets()) {
    std::cout << "\r" << target.string() << " "
              << prettyPrintBytes(fileSaver.getCurrentSizeAt(target.string()))
              << "                                                             "
                 "             "
              << std::endl;
  }

  return 0;
}

} // namespace filesaver
