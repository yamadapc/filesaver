//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#include <leveldb/write_batch.h>
#include <spdlog/spdlog.h>

#include "LevelDbFactory.h"
#include "LevelDbStorageService.h"

namespace filesaver
{

LevelDbStorageService::LevelDbStorageService (services::LevelDbFactory* levelDbFactory)
    : m_levelDbFactory (levelDbFactory)
{
    leveldb::Options options;
    if (const auto status = levelDbFactory->destroyDatabase (options, databaseTag); !status.ok ())
    {
        spdlog::warn ("Failed to destroy database on open");
    }
    options.compression = leveldb::kSnappyCompression;
    options.create_if_missing = true;
    const leveldb::Status status = levelDbFactory->openDatabase (options, databaseTag, &database);

    if (!status.ok ())
    {
        database = nullptr;
        spdlog::error ("Failed to open database, memory usage will be degraded");
    }
}

LevelDbStorageService::~LevelDbStorageService ()
{
    spdlog::warn ("Destroying default LevelDB database");
    const leveldb::Options options;
    if (const auto status = m_levelDbFactory->destroyDatabase (options, databaseTag); !status.ok ())
    {
        spdlog::warn ("Failed to destroy database on close");
    }
    delete database;
}

bool LevelDbStorageService::isDatabaseOk () const
{
    return database != nullptr;
}

int LevelDbStorageService::createTables ()
{
    return 0;
}

int LevelDbStorageService::insertEntry (const FileSizePair& pair)
{
    if (database == nullptr)
    {
        return 1;
    }

    constexpr leveldb::WriteOptions writeOptions;
    const auto output = boost::lexical_cast<std::string> (pair.getSize ());
    const leveldb::Slice value (output);

    const auto status = database->Put (writeOptions, getFileSizeKey (pair.getFilename ()), value);
    return status.ok ();
}

int LevelDbStorageService::insertEntryBatch (const std::vector<FileSizePair>& pairs, size_t start, size_t end)
{
    if (database == nullptr)
    {
        return 1;
    }

    constexpr leveldb::WriteOptions writeOptions;
    leveldb::WriteBatch batch;

    for (size_t i = start; i < end; i++)
    {
        const auto& pair = pairs[i];
        batch.Put (getFileSizeKey (pair.getFilename ()), boost::lexical_cast<std::string> (pair.getSize ()));
        spdlog::trace ("LevelDbStorageService - Writing filename={} size={}", pair.getFilename (), pair.getSize ());
    }

    const auto status = database->Write (writeOptions, &batch);
    return status.ok ();
}

std::optional<FileSizePair> LevelDbStorageService::fetchEntry (const std::string& filepath)
{
    if (database == nullptr)
    {
        return std::nullopt;
    }

    leveldb::ReadOptions readOptions;
    readOptions.fill_cache = false;
    std::string result;
    const auto status = database->Get (readOptions, getFileSizeKey (filepath), &result);

    if (!status.ok ())
    {
        return std::nullopt;
    }

    try
    {
        const auto size = boost::lexical_cast<off_t> (result);
        return {FileSizePair (filepath, size, nullptr)};
    }
    catch (const boost::bad_lexical_cast& err)
    {
        spdlog::error ("Failed to read entry err={} filepath={} result={}", err.what (), filepath, result);
        return std::nullopt;
    }
}

std::string LevelDbStorageService::getFileSizeKey (const std::string& filename)
{
    return fmt::format ("filesize::{}", filename);
}

} // namespace filesaver
