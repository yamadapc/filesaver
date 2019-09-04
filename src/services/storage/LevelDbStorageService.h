//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#ifndef FILE_SAVER_LEVELDBSTORAGESERVICE_H
#define FILE_SAVER_LEVELDBSTORAGESERVICE_H

#include <leveldb/db.h>

#include "../../models/FileEntry.h"

namespace filesaver {

class LevelDbStorageService {
public:
  explicit LevelDbStorageService(const std::string &dbFilename);
  ~LevelDbStorageService ();

  int createTables();
  int insertEntry(const FileEntry &entry);

private:
  leveldb::DB* database;
};

} // namespace filesaver

#endif // FILE_SAVER_LEVELDBSTORAGESERVICE_H
