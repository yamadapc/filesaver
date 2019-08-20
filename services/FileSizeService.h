//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILESIZESERVICE_H
#define FILE_SAVER_FILESIZESERVICE_H

#include <dirent.h>
#include <sys/stat.h>

#include <fstream>
#include <vector>

namespace filesize_service {

class FileEntry {
public:
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

  uintmax_t dev;
  uintmax_t ino;

  FileType type;
  off_t size;
  char* filename;

  FileEntry(FileType type, off_t size, uintmax_t dev, uintmax_t ino, char* filename);

    FileEntry (uintmax_t dev, uintmax_t ino, FileType type, off_t size, char *filename);

    std::vector<char *> children();

  static std::unique_ptr<FileEntry> fromPath(char *filename);
};

} // namespace filesize_service

#endif // FILE_SAVER_FILESIZESERVICE_H
