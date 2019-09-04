//
// Created by Pedro Tacla Yamada on 2019-09-02.
//
#include <SQLiteCpp/SQLiteCpp.h>
#include <boost/filesystem/path.hpp>

#include "StorageService.h"

namespace filesaver {

StorageService::StorageService(SQLite::Database &database)
    : database(database) {}

int StorageService::createTables() {
  if (database.tableExists("file_entry")) {
    return 0;
  }

  // database.exec("DROP TABLE IF EXISTS file_entry");

  SQLite::Statement createFileEntryTable(database,
                                         "CREATE TABLE file_entry ("
                                         "    filename UNIQUE PRIMARY KEY,"
                                         "    size"
                                         ");");
  return createFileEntryTable.exec();
}

int StorageService::insertEntry(const FileEntry &entry) {
  SQLite::Statement insertEntryStatement(
      database, "INSERT OR REPLACE INTO file_entry VALUES (?, ?)");

  insertEntryStatement.bindNoCopy(1, entry.filepath.string());
  insertEntryStatement.bind(2, entry.size);

  return insertEntryStatement.exec();
}

std::optional<FileEntry>
StorageService::fetchEntry(const std::string &filename) {
  SQLite::Statement query(database, "SELECT filename, ino, dev, type, size"
                                    "FROM file_entry WHERE filename = ?");
  query.bind(1, filename);

  auto success = query.executeStep();

  if (!success) {
    return {};
  }

  FileEntry fileEntry;

  fileEntry.filepath = query.getColumn(0).getString();
  fileEntry.ino = query.getColumn(1).getUInt();
  fileEntry.dev = query.getColumn(2).getUInt();
  fileEntry.type = static_cast<FileType>(query.getColumn(3).getInt());
  fileEntry.size = query.getColumn(4).getUInt();

  return {fileEntry};
}

template <class Iterator>
int StorageService::bulkInsertEntries(Iterator begin, Iterator end) {
  SQLite::Transaction transaction(database);

  while (begin != end) {
    FileEntry &fileEntry = *begin;
    insertEntry(fileEntry);
  }

  transaction.commit();
}

} // namespace filesaver
