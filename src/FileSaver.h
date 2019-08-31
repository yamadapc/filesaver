//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILESAVER_H
#define FILE_SAVER_FILESAVER_H

#include <boost/filesystem/path.hpp>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "services/FileSizeService.h"
#include "workers/WorkerManager.h"

std::string prettyPrintBytes(off_t bytes);

class FileSaver {
public:
  static int main(int argc, char *argv[]);

  FileSaver();
  ~FileSaver();

  void start();
  void stop();
  void scan(const std::string &filepath);
  void entryReader();

  off_t getCurrentSizeAt(const std::string &filepath);
  bool isPathFinished(const std::string &filepath);
  bool areAllTargetsFinished();

  const std::unordered_set<std::string> &getTargets() { return targets; }
  unsigned long getTotalFiles() { return totalFiles; }
  unsigned long getFilesPerSecond() { return filesPerSecond; }
  unsigned getNumWorkers() { return manager.getNumWorkers(); }

private:
  void addSize(const boost::filesystem::path &path, off_t sizeDiff);
  void updateSizes(const std::shared_ptr<filesize_service::FileEntry> &entry);
  void onFileSizeChanged(const std::string &filepath, off_t sizeDiff);

  std::thread readerThread;
  bool running;

  unsigned long totalFiles = 0;
  unsigned long filesPerSecond;

  WorkerManager manager;

  std::unordered_set<std::string> targets;
  std::unordered_map<std::string, std::shared_ptr<filesize_service::FileEntry>>
      allEntries;
  std::unordered_map<std::string, off_t> totalSizes;
  std::unordered_map<std::string, bool> isFinishedMap;
};

#endif // FILE_SAVER_FILESAVER_H
