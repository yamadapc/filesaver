//
// Created by Pedro Tacla Yamada on 27/4/21.
//

#include <boost/filesystem/path.hpp>
#include <fmt/format.h>

#include "LevelDbFactory.h"

namespace filesaver::services
{

LevelDbFactory::LevelDbFactory (filesaver::services::settings::SettingsService* settingsService)
    : m_settingsService (settingsService)
{
}

leveldb::Status LevelDbFactory::openDatabase (const leveldb::Options& options,
                                              const std::string& databaseTag,
                                              leveldb::DB** databasePtr)
{
    return leveldb::DB::Open (options, getPathForTag (databaseTag), databasePtr);
}

leveldb::Status LevelDbFactory::destroyDatabase (const leveldb::Options& options, const std::string& databaseTag)
{
    return leveldb::DestroyDB (getPathForTag (databaseTag), options);
}

std::string LevelDbFactory::getPathForTag (const std::string& databaseTag)
{
    boost::filesystem::path supportDir{m_settingsService->getSupportDirectoryPath ()};
    std::string databaseFilename = fmt::format ("{}.db", databaseTag);
    supportDir.append (databaseFilename);
    return supportDir.string ();
}

} // namespace filesaver::services
