//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "FileSizeService.h"

namespace filesize_service {

FileEntry::FileEntry(FileEntry::FileType type, off_t size, uintmax_t dev,
                     uintmax_t ino, char *filename)
    : dev(dev), ino(ino), type(type), size(size), filename(filename) {}

std::vector<char *> FileEntry::children() {
  std::vector<char *> childPaths;

  if (type != FileType::directory) {
    return childPaths;
  }

  auto *dir = opendir(filename);
  auto *ent = readdir(dir);

  do {
    childPaths.push_back(ent->d_name);
  } while ((ent = readdir(dir)) != nullptr);

  return childPaths;
}

std::unique_ptr<FileEntry> FileEntry::fromPath(char *filename) {
  auto *buffer = new struct stat;
  int result = lstat(filename, buffer);

  if (result != 0) {
    return nullptr;
  }

  FileType type;
  mode_t mode = buffer->st_mode;

  if (S_ISDIR(mode)) {
    type = FileType::directory;
  } else if (S_ISREG(mode)) {
    type = FileType::regular_file;
  } else if (S_ISLNK(mode)) {
    type = FileType ::symbolic_link;
  } else if (S_ISFIFO(mode)) {
    type = FileType ::fifo_special;
  } else if (S_ISBLK(mode)) {
    type = FileType::block_special;
  } else if (S_ISCHR(mode)) {
    type = FileType::character_special;
  } else {
    type = FileType::unknown;
  }

  return std::make_unique<FileEntry>(type, buffer->st_size, buffer->st_dev,
                                     buffer->st_ino, filename);
}
} // namespace filesize_service
