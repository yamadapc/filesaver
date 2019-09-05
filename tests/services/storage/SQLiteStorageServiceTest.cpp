//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#include "gtest/gtest.h"
#include <iostream>

#include "../../../src/models/FileEntry.h"
#include "../../../src/services/storage/SQLiteStorageService.h"

namespace filesaver {

TEST(SQLiteStorageServiceTest, CanCreateTheDatabase) {
  auto database = std::make_shared<SQLite::Database>(
      ":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  database->exec("DROP TABLE IF EXISTS file_entry");
  EXPECT_FALSE(database->tableExists("file_entry"));

  SQLiteStorageService storageService(database);

  storageService.createTables();

  EXPECT_TRUE(database->tableExists("file_entry"));
}

TEST(SQLiteStorageServiceTest, CanStoreFileEntries) {
  auto database = std::make_shared<SQLite::Database>(
      ":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  database->exec("DROP TABLE IF EXISTS file_entry");
  EXPECT_FALSE(database->tableExists("file_entry"));

  SQLiteStorageService storageService(database);
  storageService.createTables();

  FileEntry entry(FileType::directory, 100, 0, 0,
                  static_cast<std::string>("/my/interesting/file.txt"));
  EXPECT_EQ(entry.filepath.extension().string(), ".txt");
  EXPECT_TRUE(entry.isDirectory());

  storageService.insertEntry(entry);

  auto result = database->execAndGet("SELECT COUNT(*) FROM file_entry");
  EXPECT_EQ(result.getInt64(), 1);

  SQLite::Statement query(*database, "SELECT filename, size "
                                     "FROM file_entry");
  EXPECT_TRUE(query.executeStep());

  EXPECT_TRUE(query.getColumn(0).isText());
  EXPECT_EQ(query.getColumn(0).getString(), "/my/interesting/file.txt");
  EXPECT_EQ(query.getColumn(1).getUInt(), 100);
}

TEST(SQLiteStorageServiceTest, CanRetrieveEntriesByFilename) {
  auto database = std::make_shared<SQLite::Database>(
      ":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  database->exec("DROP TABLE IF EXISTS file_entry");
  EXPECT_FALSE(database->tableExists("file_entry"));

  SQLiteStorageService storageService(database);
  storageService.createTables();

  FileEntry entry(FileType::directory, 100, 0, 0,
                  static_cast<std::string>("/my/interesting/file.txt"));
  EXPECT_EQ(entry.filepath.extension().string(), ".txt");
  EXPECT_TRUE(entry.isDirectory());

  storageService.insertEntry(entry);

  auto result = database->execAndGet("SELECT COUNT(*) FROM file_entry");
  EXPECT_EQ(result.getInt64(), 1);

  auto fetchedEntry = storageService.fetchEntry(entry.filepath.string());
  EXPECT_EQ(entry.filepath.string(), fetchedEntry->getFilename());
  EXPECT_EQ(entry.size, fetchedEntry->getSize());
}

} // namespace filesaver
