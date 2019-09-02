//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILEENTRY_H
#define FILE_SAVER_FILEENTRY_H

#include <dirent.h>
#include <sys/stat.h>

#include <fstream>
#include <vector>

namespace filesaver {

enum class FileType {
  directory,
  regular_file,
  symbolic_link,
  socket,
  block_special,
  character_special,
  fifo_special,
  unknown,
};

class FileEntry {
public:
  uintmax_t dev;
  uintmax_t ino;

  FileType type;
  off_t size;
  std::string filename;

  FileEntry(FileType type, off_t size, uintmax_t dev, uintmax_t ino,
            std::string filename);

  bool isDirectory() { return type == FileType::directory; }
  bool getHasCachedChildren() { return hasCachedChildren; }

  const std::vector<std::string> &children();

  static std::shared_ptr<FileEntry> fromPath(const std::string& filename);

private:
  std::vector<std::string> cachedChildren;
  bool hasCachedChildren = false;
};

} // namespace filesaver

#endif //FILE_SAVER_FILEENTRY_H
