//
// Created by Pedro Tacla Yamada on 2019-09-04.
//

#include "LevelDbStorageService.h"

namespace filesaver
{

LevelDbStorageService::LevelDbStorageService (const std::string& dbFilename)
{
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open (options, dbFilename, &database);
    assert (status.ok ());
}

LevelDbStorageService::~LevelDbStorageService ()
{
    delete database;
}

int LevelDbStorageService::createTables ()
{
    return 0;
}

int LevelDbStorageService::insertEntry (const filesaver::FileEntry& entry)
{
    leveldb::WriteOptions writeOptions;

    leveldb::Slice value (boost::lexical_cast<std::string> (entry.size));

    auto status = database->Put (writeOptions, entry.filepath.string (), value);

    return status.ok ();
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
