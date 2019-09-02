//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#include "StorageService.h"
#include <SQLiteCpp/Transaction.h>
#include <boost/filesystem/path.hpp>

namespace filesaver {

StorageService::StorageService(SQLite::Database &database)
    : database(database) {}

int StorageService::createTables() {
  database.exec("DROP TABLE IF EXISTS file_entry");

  SQLite::Statement createFileEntryTable(database,
                                         "CREATE TABLE file_entry ("
                                         "    dev unsigned big int,"
                                         "    ino unsigned big int,"
                                         "    filename CHAR(4096) PRIMARY KEY,"
                                         "    size unsigned big int,"
                                         "    type tinyint,"
                                         "    is_directory bool,"
                                         "    extension CHAR(255),"
                                         "    has_final_size bool"
                                         ");");
  return createFileEntryTable.exec();
}

int StorageService::insertEntry(const FileEntry &entry) {
  SQLite::Statement insertEntryStatement(
      database, "INSERT INTO file_entry VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

  insertEntryStatement.bind(1, (long long)entry.dev);
  insertEntryStatement.bind(2, (long long)entry.ino);
  insertEntryStatement.bind(3, entry.filepath.string());
  insertEntryStatement.bind(4, entry.size);
  insertEntryStatement.bind(5, (int)entry.type);
  insertEntryStatement.bind(6, entry.isDirectory());
  insertEntryStatement.bind(7, entry.filepath.extension().string());
  insertEntryStatement.bind(8, !entry.isDirectory());

  return insertEntryStatement.exec();
}

int StorageService::updateEntrySize(const std::string &filepath, off_t delta,
                                    bool isFinished, bool updateParents) {
  SQLite::Statement updateEntryStatement(
      database, "UPDATE file_entry SET size = size + ?, has_final_size = ? "
                "WHERE filename = ?");

  updateEntryStatement.bind(1, delta);
  updateEntryStatement.bind(2, isFinished);
  updateEntryStatement.bind(3, filepath);

  auto result = updateEntryStatement.exec();

  if (updateParents) {
    boost::filesystem::path parentPath(filepath);
    while (parentPath.has_parent_path()) {
      parentPath = parentPath.parent_path();

      SQLite::Statement updateParentsStatement(
          database, "UPDATE file_entry SET size = size + ?"
                    "WHERE filename = ?");

      updateParentsStatement.bind(1, delta);
      updateParentsStatement.bind(2, filepath);

      result += updateParentsStatement.exec();
    }
  }

  return result;
}

int StorageService::updateEntryFinished(const std::string &filepath,
                                        bool isFinished) {
  SQLite::Statement updateEntryStatement(
      database, "UPDATE file_entry SET has_final_size = ? "
                "WHERE filename = ?");

  updateEntryStatement.bind(1, isFinished);
  updateEntryStatement.bind(2, filepath);

  return updateEntryStatement.exec();
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
