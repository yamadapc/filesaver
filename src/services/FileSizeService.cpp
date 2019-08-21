//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <boost/filesystem/path.hpp>

#include "FileSizeService.h"

namespace filesize_service {

FileEntry::FileEntry(FileType type, off_t size, uintmax_t dev, uintmax_t ino,
                     std::string filename)
    : dev(dev), ino(ino), type(type), size(size), filename(filename) {}

std::vector<std::string> FileEntry::children() {
  std::vector<std::string> childPaths;

  if (type != FileType::directory) {
    return childPaths;
  }

  auto *dir = opendir(filename.c_str());

  if (dir == nullptr) {
    return childPaths;
  }

  auto *ent = readdir(dir);

  if (ent == nullptr) {
    closedir(dir);
    return childPaths;
  }

  do {
    std::string file{ent->d_name};
    if (file != "." && file != "..") {
      boost::filesystem::path path{filename};
      path.append(file);
      childPaths.push_back(path.string());
    }
  } while ((ent = readdir(dir)) != nullptr);

  closedir(dir);
  free(ent);

  return childPaths;
}

std::shared_ptr<FileEntry> FileEntry::fromPath(std::string filename) {
  auto *buffer = new struct stat;
  int result = lstat(filename.c_str(), buffer);

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

  auto fileEntry = std::make_shared<FileEntry>(
      type, buffer->st_size, buffer->st_dev, buffer->st_ino, filename);
  delete buffer;
  return fileEntry;
}
} // namespace filesize_service
