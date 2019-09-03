//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#include "gtest/gtest.h"
#include <iostream>

#include "../../src/models/FileEntry.h"
#include "../../src/services/StorageService.h"

namespace filesaver {

TEST(StorageServiceTest, CanCreateTheDatabase) {
  SQLite::Database database(":memory:",
                            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  database.exec("DROP TABLE IF EXISTS file_entry");

  EXPECT_FALSE(database.tableExists("file_entry"));

  StorageService storageService(database);

  storageService.createTables();

  EXPECT_TRUE(database.tableExists("file_entry"));
}

TEST(StorageServiceTest, CanStoreFileEntries) {
  SQLite::Database database(":memory:",
                            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  database.exec("DROP TABLE IF EXISTS file_entry");
  EXPECT_FALSE(database.tableExists("file_entry"));

  StorageService storageService(database);
  storageService.createTables();

  FileEntry entry(FileType::directory, 100, 0, 0,
                  static_cast<std::string>("/my/interesting/file.txt"));
  EXPECT_EQ(entry.filepath.extension().string(), ".txt");
  EXPECT_TRUE(entry.isDirectory());

  storageService.insertEntry(entry);

  auto result = database.execAndGet("SELECT COUNT(*) FROM file_entry");
  EXPECT_EQ(result.getInt64(), 1);

  SQLite::Statement query(database, "SELECT filename, ino, dev, type, size, "
                                    "extension, is_directory FROM file_entry");
  EXPECT_TRUE(query.executeStep());

  EXPECT_TRUE(query.getColumn(0).isText());
  EXPECT_EQ(query.getColumn(0).getString(), "/my/interesting/file.txt");
  EXPECT_EQ(query.getColumn(1).getUInt(), 0);
  EXPECT_EQ(query.getColumn(2).getUInt(), 0);
  EXPECT_EQ(static_cast<FileType>(query.getColumn(3).getInt()),
            FileType::directory);
  EXPECT_EQ(query.getColumn(4).getUInt(), 100);
  EXPECT_EQ(query.getColumn(5).getString(), ".txt");
  EXPECT_EQ(query.getColumn(6).getBytes(), true);
}

TEST(StorageService, CanRetrieveEntriesByFilename) {
  SQLite::Database database(":memory:",
                            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  database.exec("DROP TABLE IF EXISTS file_entry");
  EXPECT_FALSE(database.tableExists("file_entry"));

  StorageService storageService(database);
  storageService.createTables();

  FileEntry entry(FileType::directory, 100, 0, 0,
                  static_cast<std::string>("/my/interesting/file.txt"));
  EXPECT_EQ(entry.filepath.extension().string(), ".txt");
  EXPECT_TRUE(entry.isDirectory());

  storageService.insertEntry(entry);

  auto result = database.execAndGet("SELECT COUNT(*) FROM file_entry");
  EXPECT_EQ(result.getInt64(), 1);

  auto fetchedEntry = storageService.fetchEntry(entry.filepath.string());
  EXPECT_EQ(entry, *fetchedEntry);
}

} // namespace filesaver
