//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#include "LevelDbStorageService.h"
#include <boost/uuid/detail/md5.hpp>

namespace filesaver
{

LevelDbStorageService::LevelDbStorageService (const std::string& dbFilename)
{
    leveldb::Options options;
    leveldb::DestroyDB (dbFilename, options);
    m_dbFilename = dbFilename;
    options.compression = leveldb::kSnappyCompression;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open (options, dbFilename, &database);
    assert (status.ok ());
}

LevelDbStorageService::~LevelDbStorageService ()
{
    leveldb::Options options;
    leveldb::DestroyDB (m_dbFilename, options);
    delete database;
}

int LevelDbStorageService::createTables ()
{
    return 0;
}

int LevelDbStorageService::insertEntry (const FileSizePair& pair)
{
    leveldb::WriteOptions writeOptions;
    // writeOptions.sync = true;
    leveldb::Slice value (boost::lexical_cast<std::string> (pair.getSize ()));

    auto status = database->Put (writeOptions, pair.getFilename (), value);
    return static_cast<int> (status.ok ());
}

std::optional<FileSizePair> LevelDbStorageService::fetchEntry (const std::string& filepath)
{
    leveldb::ReadOptions readOptions;
    std::string result;
    auto status = database->Get (readOptions, filepath, &result);

    if (!status.ok ())
    {
        return {};
    }

    auto size = boost::lexical_cast<off_t> (result);
    return {FileSizePair (filepath, size)};
}

} // namespace filesaver
