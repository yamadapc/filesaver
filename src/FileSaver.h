//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILESAVER_H
#define FILE_SAVER_FILESAVER_H

#include <boost/filesystem/path.hpp>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "models/FileEntry.h"
#include "services/storage/LevelDbStorageService.h"
#include "workers/WorkerManager.h"

namespace filesaver {

std::string prettyPrintBytes(off_t bytes);

class FileSaver {
public:
  FileSaver();
  ~FileSaver();

  void start();
  void stop();
  void scan(const std::string &filepath);
  void entryReader();
  void entryWriter();

  off_t getCurrentSizeAt(const std::string &filepath);
  bool isPathFinished(boost::filesystem::path &filepath);
  bool areAllTargetsFinished();
  void setupDefaultStorage();

  std::vector<boost::filesystem::path> getTargets();
  unsigned long getTotalFiles();
  double getFilesPerSecond();
  unsigned long getNumWorkers();
  long long int getElapsed();
  size_t getStorageQueueSize();
  size_t getInMemoryEntryCount();

private:
  void updateSizes(const std::shared_ptr<filesaver::FileEntry> &entry);
  void onFileSizeChanged(const boost::filesystem::path &filepath,
                         off_t sizeDiff);
  void onFinished(const boost::filesystem::path &filepath);
  void addSize(const boost::filesystem::path &path, off_t sizeDiff);

  bool hasStorage();

  WorkQueue<std::shared_ptr<FileEntry>> storageQueue;
  std::unordered_map<std::string, std::shared_ptr<FileEntry>> allEntries;
  std::unordered_map<std::string, off_t> totalSizes;
  std::unordered_map<std::string, unsigned> pendingChildren;

  std::vector<boost::filesystem::path> targets;

  unsigned long totalFiles = 0;
  double filesPerSecond = 0.0;

  std::unique_ptr<LevelDbStorageService> storageService;
  bool running = false;
  bool storing = false;
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
      startTime;
  std::thread storageThread;
  std::thread readerThread;
  WorkerManager manager;
};

} // namespace filesaver

#endif // FILE_SAVER_FILESAVER_H
