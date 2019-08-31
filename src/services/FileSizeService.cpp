#include <utility>

//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <boost/filesystem/path.hpp>

#include "FileSizeService.h"

namespace filesize_service {

FileEntry::FileEntry(FileType type, off_t size, uintmax_t dev, uintmax_t ino,
                     std::string filename)
    : dev(dev), ino(ino), type(type), size(size),
      filename(std::move(filename)) {}

const std::vector<std::string> &FileEntry::children() {
  if (hasCachedChildren) {
    return cachedChildren;
  }

  if (type != FileType::directory) {
    hasCachedChildren = true;
    return cachedChildren;
  }

  auto *dir = opendir(filename.c_str());

  if (dir == nullptr) {
    hasCachedChildren = true;
    return cachedChildren;
  }

  auto *ent = readdir(dir);
  if (ent == nullptr) {
    closedir(dir);
    hasCachedChildren = true;
    return cachedChildren;
  }

  {
    std::vector<std::string> childPaths;

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

    cachedChildren = childPaths;
    hasCachedChildren = true;
    return cachedChildren;
  }
}

std::shared_ptr<FileEntry> FileEntry::fromPath(std::string filename) {
  struct stat buffer;
  int result = lstat(filename.c_str(), &buffer);

  if (result != 0) {
    return std::make_shared<FileEntry>(FileType::unknown, 0, 0, 0, filename);
  }

  FileType type;
  mode_t mode = buffer.st_mode;

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
      type, buffer.st_size, buffer.st_dev, buffer.st_ino, std::move(filename));

  return fileEntry;
}

} // namespace filesize_service
