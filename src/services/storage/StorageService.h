//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#ifndef FILE_SAVER_STORAGESERVICE_H
#define FILE_SAVER_STORAGESERVICE_H

#include <optional>

#include "../../models/FileEntry.h"

namespace filesaver {

class StorageService {
public:
  virtual ~StorageService() {};

  virtual int createTables() = 0;
  virtual int insertEntry(const FileEntry &entry) = 0;
  virtual std::optional<FileSizePair>
  fetchEntry(const std::string &filepath) = 0;
};

} // namespace filesaver

#endif // FILE_SAVER_STORAGESERVICE_H
