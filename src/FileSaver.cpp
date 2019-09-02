//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <boost/format.hpp>
#include <chrono>
#include <thread>
#include <unordered_map>

#include "FileSaver.h"
#include "workers/WorkerManager.h"

namespace filesaver {

std::string prettyPrintBytes(off_t bytes) {
  const char *suffixes[7] = {
      "B", "KB", "MB", "GB", "TB", "PB", "EB",
  };
  uint suffixIndex = 0;
  double count = bytes;
  while (count >= 1024 && suffixIndex < 7) {
    suffixIndex++;
    count /= 1024;
  }

  if (count - floor(count) == 0.0) {
    return (boost::format("%d%s") % count % suffixes[suffixIndex]).str();
  }

  return (boost::format("%.1f%s") % count % suffixes[suffixIndex]).str();
}

int FileSaver::main(int argc, char *argv[]) {
  std::cout << "file-saver" << std::endl;
  FileSaver fileSaver;

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
    for (auto &target : fileSaver.getTargets()) {
      std::cout << "\rWorking... " << target << " - "
                << prettyPrintBytes(fileSaver.getCurrentSizeAt(target)) << " - "
                << fileSaver.getTotalFiles() << " files scanned - "
                << "                                   ";
      std::cout.flush();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }

  for (auto &target : fileSaver.getTargets()) {
    std::cout << "\r" << target << " "
              << prettyPrintBytes(fileSaver.getCurrentSizeAt(target))
              << "                                                             "
                 "             "
              << std::endl;
  }

  return 0;
}

FileSaver::FileSaver() = default;

FileSaver::~FileSaver() { stop(); }

void FileSaver::start() {
  unsigned int numCpus;
  auto *numWorkersEnv = std::getenv("NUM_WORKERS");

  if (numWorkersEnv) {
    numCpus = static_cast<unsigned int>(abs (std::stoi (numWorkersEnv)));
  } else {
    numCpus = std::thread::hardware_concurrency() * 2;
  }

  std::cout << "Starting with " << numCpus << " threads" << std::endl;

  running = true;
  manager.start(numCpus);
  readerThread = std::thread(&FileSaver::entryReader, this);
}

void FileSaver::stop() {
  manager.stop();
  running = false;
  readerThread.join();
}

void FileSaver::scan(const std::string &filepath) {
  auto target = filepath != "/" ? boost::filesystem::path{filepath}
                                      .remove_trailing_separator()
                                      .string()
                                : filepath;
  if (targets.find(target) == targets.end()) {
    manager.scan(target);
    targets.insert(target);
  }
}

off_t FileSaver::getCurrentSizeAt(const std::string &filepath) {
  auto totalSizeIt = totalSizes.find(filepath);

  if (totalSizeIt == totalSizes.end()) {
    return 0;
  }

  return totalSizeIt->second;
}

bool FileSaver::isPathFinished(const std::string &filepath) {
  auto isFinishedIt = isFinishedMap.find(filepath);
  if (isFinishedIt != isFinishedMap.end()) {
    return isFinishedIt->second;
  }

  auto entryIt = allEntries.find(filepath);
  if (entryIt == allEntries.end()) {
    return false;
  }

  auto fileEntry = entryIt->second;
  if (!fileEntry->isDirectory()) {
    isFinishedMap[filepath] = true;
    return true;
  }

  if (!fileEntry->getHasCachedChildren()) {
    return false;
  }

  for (const auto &child : fileEntry->children()) {
    if (!isPathFinished(child)) {
      return false;
    }
  }

  isFinishedMap[filepath] = true;
  return true;
}

void FileSaver::entryReader() {
  unsigned long iterations = 0;
  auto now = std::chrono::high_resolution_clock::now();

  while (running) {
    while (manager.resultQueue.size() > 0) {
      auto entry = manager.resultQueue.front();
      updateSizes(entry);
      allEntries[entry->filename] = std::move(entry);
      totalFiles += 1;
    }

    auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now() - now)
                       .count();

    filesPerSecond =
        0.0 != seconds ? 1000.0 * ((double)totalFiles / seconds) : 0.0;
    iterations += 1;

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}

void FileSaver::updateSizes(const std::shared_ptr<FileEntry> &entry) {
  auto previousSize = getCurrentSizeAt(entry->filename);
  auto sizeDiff = entry->size - previousSize;
  onFileSizeChanged(entry->filename, sizeDiff);
}

void FileSaver::addSize(const boost::filesystem::path &path, off_t sizeDiff) {
  auto parentIt = totalSizes.find(path.string());
  if (parentIt != totalSizes.end()) {
    totalSizes[path.string()] += sizeDiff;
  } else {
    totalSizes[path.string()] = sizeDiff;
  }
}

void FileSaver::onFileSizeChanged(const std::string &filepath, off_t sizeDiff) {
  boost::filesystem::path path{filepath};

  addSize(path, sizeDiff);

  while (path.has_parent_path()) {
    path = path.parent_path();
    addSize(path, sizeDiff);
  }
}

bool FileSaver::areAllTargetsFinished() {
  for (auto &target : targets) {
    if (!isPathFinished(target)) {
      return false;
    }
  }
  return true;
}

} // namespace filesaver
