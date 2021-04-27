//
// Created by Pedro Tacla Yamada on 27/4/21.
//

#include "LevelDbFileCategoryStore.h"
#include <spdlog/spdlog.h>

namespace filesaver::services
{

LevelDbFileCategoryStore::LevelDbFileCategoryStore (LevelDbFactory* factory) : m_levelDbFactory (factory)
{
}

LevelDbFileCategoryStore::~LevelDbFileCategoryStore ()
{
    for (const auto& pair : m_categoryDBStore)
    {
        leveldb::DB* db = pair.second;
        delete db;
    }
}

void LevelDbFileCategoryStore::insertPath (const std::string& categoryTag, const std::string& filepath)
{
    auto* db = getDatabase (categoryTag);
    leveldb::WriteOptions options;

    auto status = db->Put (options, filepath, "");
    if (!status.ok ())
    {
        throw status;
    }
}

std::vector<std::string> LevelDbFileCategoryStore::getPaths (const std::string& categoryTag, long limit, long offset)
{
    auto* db = getDatabase (categoryTag);
    leveldb::ReadOptions options;

    std::vector<std::string> result;
    auto* it = db->NewIterator (options);

    off_t index = 0;
    off_t limitIndex = offset + (limit - 1);
    for (it->SeekToFirst (); it->Valid (); it->Next (), index += 1)
    {
        if (index < offset)
        {
            continue;
        }

        if (index > limitIndex)
        {
            break;
        }

        result.push_back (it->key ().ToString ());
    }

    auto status = it->status ();
    delete it;

    if (!status.ok ())
    {
        throw status;
    }

    return result;
}

leveldb::DB* LevelDbFileCategoryStore::getDatabase (const std::string& categoryTag)
{
    if (m_categoryDBStore.find (categoryTag) != m_categoryDBStore.end ())
    {
        return m_categoryDBStore[categoryTag];
    }

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    auto status = m_levelDbFactory->openDatabase (options, categoryTag, &db);
    if (!status.ok ())
    {
        throw status;
    }

    m_categoryDBStore[categoryTag] = db;
    return db;
}

} // namespace filesaver::services