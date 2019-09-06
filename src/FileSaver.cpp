//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <SQLiteCpp/Transaction.h>
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

FileSaver::FileSaver(){};
FileSaver::~FileSaver() { stop(); }

void FileSaver::start() {
  startTime = std::chrono::steady_clock::now();

  unsigned int numCpus;
  auto *numWorkersEnv = std::getenv("NUM_WORKERS");

  if (numWorkersEnv) {
    numCpus = static_cast<unsigned int>(abs(std::stoi(numWorkersEnv)));
  } else {
    numCpus = std::thread::hardware_concurrency() * 2;
  }

  std::cout << "Starting with " << numCpus << " threads" << std::endl;

  running = true;
  manager.start(numCpus);
  readerThread = std::thread(&FileSaver::entryReader, this);

  if (hasStorage()) {
    storageService->createTables();
    storageThread = std::thread(&FileSaver::entryWriter, this);
  }
}

void FileSaver::stop() {
  if (!running) {
    return;
  }

  manager.stop();
  running = false;
  readerThread.join();

  std::cout << "Storing results..." << std::endl;
  storing = true;
  storageThread.join();
}

void FileSaver::setupDefaultStorage() {
  storageService = std::make_unique<LevelDbStorageService>("filesaver.db");
}

void FileSaver::scan(const std::string &filepath) {
  auto target =
      filepath != "/"
          ? boost::filesystem::path{filepath}.remove_trailing_separator()
          : "/";
  if (std::find(targets.begin(), targets.end(), target) == targets.end()) {
    manager.scan(target.string());
    targets.push_back(target);
  }
}

off_t FileSaver::getCurrentSizeAt(const std::string &filepath) {
  auto totalSizeIt = totalSizes.find(filepath);

  if (totalSizeIt == totalSizes.end()) {
    return 0;
  }

  return totalSizeIt->second;
}

bool FileSaver::isPathFinished(boost::filesystem::path &filepath) {
  const auto &filepathStr = filepath.string();
  auto pendingIt = pendingChildren.find(filepathStr);

  if (pendingIt == pendingChildren.end()) {
    return false;
  }

  return pendingIt->second <= 0;
}

void FileSaver::entryReader() {
  unsigned long iterations = 0;

  while (running) {
    while (manager.resultQueue.size() > 0) {
      auto entry = manager.resultQueue.front();
      auto filepath = entry->filepath;
      auto filepathStr = entry->filepath.string();

      allEntries[filepathStr] = entry;

      if (!entry->isDirectory()) {
        pendingChildren[filepathStr] = 0;
        onFinished(entry->filepath);
      } else {
        pendingChildren[filepathStr] = entry->children().size();
        if (pendingChildren[filepathStr] == 0) {
          onFinished(entry->filepath);
        }
      }
      updateSizes(entry);
      totalFiles += 1;
      totalKnownFiles += entry->children().size();
    }

    iterations += 1;

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}

void FileSaver::entryWriter() {
  while (running) {
    // SQLite::Transaction transaction(database);
    unsigned long entries = 0;
    while (storageQueue.size() > 0) {
      auto entry = storageQueue.front();
      storageService->insertEntry(*entry);
      entries += 1;
    }
    if (storing) {
      std::cout << "Flushing " << entries << " entries..." << std::endl;
    }
    // transaction.commit();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void FileSaver::onFinished(const boost::filesystem::path &filepath) {
  const auto &filepathStr = filepath.string();
  auto entry = allEntries[filepathStr];
  entry->isFinished = true;
  allEntries.erase(filepathStr);

  if (hasStorage()) {
    storageQueue.push(std::move(entry));
  }

  if (filepath.has_parent_path()) {
    auto parentPath = filepath.parent_path();
    const auto &parentPathStr = parentPath.string();

    pendingChildren[parentPathStr] -= 1;
    if (pendingChildren[parentPathStr] <= 0) {
      onFinished(parentPath);
    }
  }
}

void FileSaver::addSize(const boost::filesystem::path &path, off_t sizeDiff) {
  auto parentIt = totalSizes.find(path.string());
  if (parentIt != totalSizes.end()) {
    totalSizes[path.string()] += sizeDiff;
  } else {
    totalSizes[path.string()] = sizeDiff;
  }
}

void FileSaver::updateSizes(const std::shared_ptr<FileEntry> &entry) {
  auto previousSize = getCurrentSizeAt(entry->filepath.string());
  auto sizeDiff = entry->size - previousSize;
  onFileSizeChanged(entry->filepath, sizeDiff);
}

void FileSaver::onFileSizeChanged(const boost::filesystem::path &filepath,
                                  off_t sizeDiff) {
  addSize(filepath, sizeDiff);

  boost::filesystem::path path = {filepath};
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

std::vector<boost::filesystem::path> FileSaver::getTargets() { return targets; }

unsigned long FileSaver::getTotalFiles() { return totalFiles; }
unsigned long FileSaver::getTotalKnownFiles() { return totalKnownFiles; }

double FileSaver::getFilesPerSecond() { return filesPerSecond; }

unsigned long FileSaver::getNumWorkers() { return manager.getNumWorkers(); }

long long int FileSaver::getElapsed() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now() - startTime)
      .count();
}

size_t FileSaver::getStorageQueueSize() { return storageQueue.size(); }

size_t FileSaver::getInMemoryEntryCount() { return allEntries.size(); }

bool FileSaver::hasStorage() { return storageService != nullptr; }

} // namespace filesaver
