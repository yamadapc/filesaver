//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#ifndef FILE_SAVER_STORAGESERVICE_H
#define FILE_SAVER_STORAGESERVICE_H

#include <SQLiteCpp/Database.h>
#include <optional>

#include "../models/FileEntry.h"

namespace filesaver {

class StorageService {
public:
  explicit StorageService(SQLite::Database &database);

  int createTables();
  int insertEntry(const FileEntry &entry);
  int updateEntrySize(const std::string &filepath, off_t delta, bool isFinished,
                      bool updateParents);
  int updateEntryFinished(const std::string &filepath, bool isFinished);

  std::optional<FileEntry> fetchEntry(const std::string &filepath);

  template <class Iterator> int bulkInsertEntries(Iterator begin, Iterator end);

private:
  SQLite::Database &database;
};

} // namespace filesaver

#endif // FILE_SAVER_STORAGESERVICE_H
