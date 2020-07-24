//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#include "LevelDbStorageService.h"
#include <boost/uuid/detail/md5.hpp>
#include <leveldb/write_batch.h>
#include <spdlog/spdlog.h>

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

    if (!status.ok ())
    {
        database = nullptr;
        spdlog::error ("Failed to open database, memory usage will be degraded");
    }
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
    leveldb::Slice value (boost::lexical_cast<std::string> (pair.getSize ()));

    auto status = database->Put (writeOptions, pair.getFilename (), value);
    return static_cast<int> (status.ok ());
}

int LevelDbStorageService::insertEntryBatch (const std::vector<FileSizePair>& pairs, size_t start, size_t end)
{
    leveldb::WriteOptions writeOptions;
    leveldb::WriteBatch batch;

    for (size_t i = start; i < end; i++)
    {
        const auto& pair = pairs[i];
        batch.Put (pair.getFilename (), boost::lexical_cast<std::string> (pair.getSize ()));
        spdlog::trace ("LevelDbStorageService - Writing filename={} size={}", pair.getFilename (), pair.getSize ());
    }

    auto status = database->Write (writeOptions, &batch);
    return static_cast<int> (status.ok ());
}

std::optional<FileSizePair> LevelDbStorageService::fetchEntry (const std::string& filepath)
{
    if (database == nullptr)
    {
        return {};
    }

    leveldb::ReadOptions readOptions;
    readOptions.fill_cache = false;
    std::string result;
    auto status = database->Get (readOptions, filepath, &result);

    if (!status.ok ())
    {
        return {};
    }

    try
    {
        auto size = boost::lexical_cast<off_t> (result);
        return {FileSizePair (filepath, size)};
    }
    catch (const boost::bad_lexical_cast& err)
    {
        spdlog::error ("Failed to read entry err={} filepath={} result={}", err.what (), filepath, result);
        return {};
    }
}

} // namespace filesaver
