//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "gtest/gtest.h"
#include <iostream>

#include "../src/services/FileSizeService.h"

TEST(FileSizeServiceTest, CanTellTheSizeOfFiles) {
  auto entry = filesize_service::FileEntry::fromPath(
      "/Users/yamadapc/file-saver/main.cpp");
  auto size = entry->size;
  std::cout << std::endl << "Output:" << std::endl;
  std::cout << size << std::endl;
}

TEST(FileSizeServiceTest, CanTellTheSizeOfDirectories) {
  auto entry =
      filesize_service::FileEntry::fromPath("/Users/yamadapc/file-saver/");
  auto size = entry->size;
  std::cout << std::endl << "Output:" << std::endl;
  std::cout << size << std::endl;
}

TEST(FileSizeServiceTest, CanListDirectoryChildren) {
    auto entry =
            filesize_service::FileEntry::fromPath("/Users/yamadapc/file-saver/");
    auto children = entry->children ();

    for (auto child: children) {
        std::cout << child << std::endl;
    }
}
