//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILEENTRY_H
#define FILE_SAVER_FILEENTRY_H

#include <dirent.h>
#include <sys/stat.h>

#include <boost/filesystem/path.hpp>
#include <fstream>
#include <vector>

namespace filesaver {

enum class FileType {
  directory = 0,
  regular_file = 1,
  symbolic_link = 2,
  socket = 3,
  block_special = 4,
  character_special = 5,
  fifo_special = 6,
  unknown = 7,
};

class FileEntry {
public:
  uintmax_t dev = 0;
  uintmax_t ino = 0;

  FileType type = FileType::unknown;
  off_t size = 0;
  boost::filesystem::path filepath;

  FileEntry() = default;

  FileEntry(FileType type, off_t size, uintmax_t dev, uintmax_t ino,
            std::string filename);

  FileEntry(FileType type, off_t size, uintmax_t dev, uintmax_t ino,
              boost::filesystem::path filename);

  bool operator==(const FileEntry &rhs) const;

  bool operator!=(const FileEntry &rhs) const;

  bool isDirectory() const { return type == FileType::directory; }
  bool getHasCachedChildren() const { return hasCachedChildren; }

  const std::vector<boost::filesystem::path> &children();

  static std::shared_ptr<FileEntry> fromPath(const boost::filesystem::path &filepath);

private:
  std::vector<boost::filesystem::path> cachedChildren;
  bool hasCachedChildren = false;
};

} // namespace filesaver

#endif // FILE_SAVER_FILEENTRY_H
