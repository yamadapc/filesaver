//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "gtest/gtest.h"
#include <iostream>

#include "../../src/models/FileEntry.h"

TEST(FileEntryTest, CanTellTheSizeOfFiles) {
  auto entry =
      filesaver::FileEntry::fromPath("/Users/yamadapc/file-saver/main.cpp");
  auto size = entry->size;
  std::cout << std::endl << "Output:" << std::endl;
  std::cout << size << std::endl;
}

TEST(FileEntryTest, CanTellTheSizeOfDirectories) {
  auto entry = filesaver::FileEntry::fromPath("/Users/yamadapc/file-saver/");
  auto size = entry->size;
  std::cout << std::endl << "Output:" << std::endl;
  std::cout << size << std::endl;
}

TEST(FileEntryTest, CanListDirectoryChildren) {
  auto entry = filesaver::FileEntry::fromPath("/Users/yamadapc/file-saver/");
  auto children = entry->children();

  for (auto child : children) {
    std::cout << child << std::endl;
  }
}
