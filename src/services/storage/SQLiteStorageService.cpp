//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include <SQLiteCpp/SQLiteCpp.h>
#include <boost/filesystem/path.hpp>

#include "SQLiteStorageService.h"

namespace filesaver {

SQLiteStorageService::SQLiteStorageService(const std::string &databaseFilepath)
    : database(std::make_shared<SQLite::Database>(
          databaseFilepath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)) {}

SQLiteStorageService::SQLiteStorageService(
    std::shared_ptr<SQLite::Database> database)
    : database(database) {}

int SQLiteStorageService::createTables() {
  if (database->tableExists("file_entry")) {
    return 0;
  }

  SQLite::Statement createFileEntryTable(*database,
                                         "CREATE TABLE file_entry ("
                                         "    filename UNIQUE PRIMARY KEY,"
                                         "    size"
                                         ");");
  return createFileEntryTable.exec();
}

int SQLiteStorageService::insertEntry(const FileEntry &entry) {
  SQLite::Statement insertEntryStatement(
      *database, "INSERT OR REPLACE INTO file_entry VALUES (?, ?)");

  insertEntryStatement.bindNoCopy(1, entry.filepath.string());
  insertEntryStatement.bind(2, entry.size);

  return insertEntryStatement.exec();
}

std::optional<FileSizePair>
SQLiteStorageService::fetchEntry(const std::string &filename) {
  SQLite::Statement query(
      *database, "SELECT filename, size FROM file_entry WHERE filename = ?");
  query.bind(1, filename);

  auto success = query.executeStep();

  if (!success) {
    return {};
  }

  FileSizePair fileSizePair{query.getColumn(0).getString(),
                            query.getColumn(1).getUInt()};
  return {fileSizePair};
}

template <class Iterator>
int SQLiteStorageService::bulkInsertEntries(Iterator begin, Iterator end) {
  SQLite::Transaction transaction(*database);

  while (begin != end) {
    FileEntry &fileEntry = *begin;
    insertEntry(fileEntry);
  }

  transaction.commit();
}

} // namespace filesaver
