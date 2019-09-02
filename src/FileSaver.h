//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILESAVER_H
#define FILE_SAVER_FILESAVER_H

#include <boost/filesystem/path.hpp>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "models/FileEntry.h"
#include "workers/WorkerManager.h"

namespace filesaver {

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
  double getFilesPerSecond() { return filesPerSecond; }
  unsigned long getNumWorkers() { return manager.getNumWorkers(); }

private:
  void addSize(const boost::filesystem::path &path, off_t sizeDiff);
  void updateSizes(const std::shared_ptr<filesaver::FileEntry> &entry);
  void onFileSizeChanged(const std::string &filepath, off_t sizeDiff);

  std::thread readerThread;
  bool running = false;

  unsigned long totalFiles = 0;
  double filesPerSecond = 0.0;

  WorkerManager manager;

  std::unordered_set<std::string> targets;
  std::unordered_map<std::string, std::shared_ptr<filesaver::FileEntry>>
      allEntries;
  std::unordered_map<std::string, off_t> totalSizes;
  std::unordered_map<std::string, bool> isFinishedMap;
};

} // namespace filesaver

#endif // FILE_SAVER_FILESAVER_H
