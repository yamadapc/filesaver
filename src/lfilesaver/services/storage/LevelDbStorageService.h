//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#ifndef FILE_SAVER_LEVELDBSTORAGESERVICE_H
#define FILE_SAVER_LEVELDBSTORAGESERVICE_H

#include <boost/lexical_cast.hpp>
#include <fruit/fruit.h>
#include <leveldb/db.h>
#include <sstream>
#include <string>

#include "../../models/FileEntry.h"
#include "./StorageService.h"

namespace filesaver
{

/// LevelDB implementation of the size cache
class LevelDbStorageService : public StorageService
{
public:
    INJECT (LevelDbStorageService (const std::string& dbFilename));
    ~LevelDbStorageService () override;

    int createTables () override;
    int insertEntry (const FileSizePair& pair) override;
    int insertEntryBatch (const std::vector<FileSizePair>& pairs, size_t start, size_t end) override;
    std::optional<FileSizePair> fetchEntry (const std::string& filepath) override;

private:
    leveldb::DB* database;
    std::string m_dbFilename;
};

} // namespace filesaver

#endif // FILE_SAVER_LEVELDBSTORAGESERVICE_H
