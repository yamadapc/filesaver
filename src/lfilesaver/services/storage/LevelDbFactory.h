//
// Created by Pedro Tacla Yamada on 27/4/21.
//

#ifndef FILESAVER_LEVELDBFACTORY_H
#define FILESAVER_LEVELDBFACTORY_H

#include <fruit/fruit.h>
#include <leveldb/db.h>
#include <string>

#include "../settings/SettingsService.h"

namespace filesaver::services
{

class LevelDbFactory
{
public:
    INJECT (LevelDbFactory (settings::SettingsService* settingsService));

    leveldb::Status
    openDatabase (const leveldb::Options& options, const std::string& databaseTag, leveldb::DB** databasePtr) const;
    leveldb::Status destroyDatabase (const leveldb::Options& options, const std::string& databaseTag) const;

    std::string getPathForTag (const std::string& databaseTag) const;

private:
    settings::SettingsService* m_settingsService;
};

} // namespace filesaver::services

#endif // FILESAVER_LEVELDBFACTORY_H
