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
#include "services/storage/LevelDbStorageService.h"
#include "workers/WorkerManager.h"

namespace filesaver {

std::string prettyPrintBytes(off_t bytes);

class FileSaver {
public:
  static int main(int argc, char *argv[]);

  FileSaver(const std::string &dbFilename);
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

  std::vector<boost::filesystem::path> getTargets() { return targets; }
  unsigned long getTotalFiles() { return totalFiles; }
  double getFilesPerSecond() { return filesPerSecond; }
  unsigned long getNumWorkers() { return manager.getNumWorkers(); }

private:
  void updateSizes(const std::shared_ptr<filesaver::FileEntry> &entry);
  void onFileSizeChanged(const boost::filesystem::path &filepath,
                         off_t sizeDiff);
  void onFinished(const boost::filesystem::path &filepath);
  void addSize(const boost::filesystem::path &path, off_t sizeDiff);

  LevelDbStorageService storageService;
  // SQLite::Database database;

  std::thread storageThread;
  std::thread readerThread;
  bool running = false;
  bool storing = false;

  unsigned long totalFiles = 0;
  double filesPerSecond = 0.0;

  WorkerManager manager;

  WorkQueue<std::shared_ptr<FileEntry>> storageQueue;
  std::vector<boost::filesystem::path> targets;
  std::unordered_map<std::string, std::shared_ptr<FileEntry>> allEntries;
  std::unordered_map<std::string, off_t> totalSizes;
  std::unordered_map<std::string, unsigned> pendingChildren;
};

} // namespace filesaver

#endif // FILE_SAVER_FILESAVER_H
