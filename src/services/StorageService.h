//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#ifndef FILE_SAVER_STORAGESERVICE_H
#define FILE_SAVER_STORAGESERVICE_H

#include "../models/FileEntry.h"
#include <SQLiteCpp/Database.h>

namespace filesaver {

class StorageService {
public:
  explicit StorageService(SQLite::Database &database);

  int createTables();
  int insertEntry(const FileEntry &entry);
  FileEntry fetchEntry(const std::string &filepath);

private:
  SQLite::Database &database;
};

} // namespace filesaver

#endif // FILE_SAVER_STORAGESERVICE_H
