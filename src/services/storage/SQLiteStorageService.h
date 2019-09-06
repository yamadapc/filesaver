//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILE_SAVER_SQLITESTORAGESERVICE_H
#define FILE_SAVER_SQLITESTORAGESERVICE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <optional>

#include "../../models/FileEntry.h"
#include "StorageService.h"

namespace filesaver {

class SQLiteStorageService : StorageService {
public:
  explicit SQLiteStorageService(const std::string &databaseFilepath);
  ~SQLiteStorageService() override = default;

  int createTables() override;
  int insertEntry(const FileEntry &entry) override;
  std::optional<FileSizePair> fetchEntry(const std::string &filepath) override;

  // SQLite specific
  explicit SQLiteStorageService(std::shared_ptr<SQLite::Database> database);
  template <class Iterator> int bulkInsertEntries(Iterator begin, Iterator end);

private:
  std::shared_ptr<SQLite::Database> database;
};

} // namespace filesaver

#endif // FILE_SAVER_SQLITESTORAGESERVICE_H
