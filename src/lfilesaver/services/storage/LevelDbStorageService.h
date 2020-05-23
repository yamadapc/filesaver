//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#ifndef FILE_SAVER_LEVELDBSTORAGESERVICE_H
#define FILE_SAVER_LEVELDBSTORAGESERVICE_H

#include <boost/lexical_cast.hpp>
#include <leveldb/db.h>
#include <sstream>
#include <string>

#include "../../models/FileEntry.h"
#include "StorageService.h"

namespace filesaver
{

/// LevelDB implementation of the size cache
class LevelDbStorageService : StorageService
{
public:
    explicit LevelDbStorageService (const std::string& dbFilename);
    ~LevelDbStorageService () override;

    int createTables () override;
    int insertEntry (const FileEntry& entry) override;
    std::optional<FileSizePair> fetchEntry (const std::string& filepath) override;

private:
    leveldb::DB* database;
};

} // namespace filesaver

#endif // FILE_SAVER_LEVELDBSTORAGESERVICE_H
