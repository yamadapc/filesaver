//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#include "StorageService.h"

namespace filesaver {

StorageService::StorageService(SQLite::Database &database)
    : database(database) {}

int StorageService::createTables() {
  SQLite::Statement createFileEntryTable(database,
                                         "CREATE TABLE file_entry ("
                                         "    dev unsigned big int,"
                                         "    ino unsigned big int,"
                                         "    filename CHAR(4096) PRIMARY KEY,"
                                         "    size unsigned big int,"
                                         "    type tinyint,"
                                         "    is_directory bool,"
                                         "    extension CHAR(255)"
                                         ");");
  return createFileEntryTable.exec();
}

int StorageService::insertEntry(const FileEntry &entry) {
  SQLite::Statement insertEntryStatement(
      database, "INSERT INTO file_entry VALUES (?, ?, ?, ?, ?, ?, ?)");

  insertEntryStatement.bind(1, (long long)entry.dev);
  insertEntryStatement.bind(2, (long long)entry.ino);
  insertEntryStatement.bind(3, entry.filename);
  insertEntryStatement.bind(4, entry.size);
  insertEntryStatement.bind(5, (int)entry.type);
  insertEntryStatement.bind(6, entry.isDirectory());
  insertEntryStatement.bind(7, entry.extension);

  return insertEntryStatement.exec();
}

FileEntry StorageService::fetchEntry(const std::string &filename) {
  SQLite::Statement query(
      database, "SELECT filename, ino, dev, type, size, "
                "extension, is_directory FROM file_entry WHERE filename = ?");
  query.bind(1, filename);

  auto success = query.executeStep();
  assert(success);

  FileEntry fileEntry;

  fileEntry.filename = query.getColumn(0).getString();
  fileEntry.ino = query.getColumn(1).getUInt();
  fileEntry.dev = query.getColumn(2).getUInt();
  fileEntry.type = static_cast<FileType>(query.getColumn(3).getInt());
  fileEntry.size = query.getColumn(4).getUInt();
  fileEntry.extension = query.getColumn(5).getString();

  return fileEntry;
}

} // namespace filesaver
