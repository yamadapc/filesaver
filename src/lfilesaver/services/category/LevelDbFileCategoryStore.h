//
// Created by Pedro Tacla Yamada on 27/4/21.
//

#ifndef FILESAVER_LEVELDBFILECATEGORYSTORE_H
#define FILESAVER_LEVELDBFILECATEGORYSTORE_H

#include <fruit/fruit.h>
#include <leveldb/db.h>
#include <string>
#include <vector>

#include "../storage/LevelDbFactory.h"

namespace filesaver::services
{

class FileCategoryStore
{
public:
    virtual ~FileCategoryStore () = default;
    virtual void insertPath (const std::string& categoryTag, const std::string& filepath) = 0;
    virtual std::vector<std::string> getPaths (const std::string& categoryTag, long limit, long offset) = 0;
};

class LevelDbFileCategoryStore : public FileCategoryStore
{
public:
    INJECT (LevelDbFileCategoryStore (LevelDbFactory* factory));
    ~LevelDbFileCategoryStore () override;

    void insertPath (const std::string& categoryTag, const std::string& filepath) override;
    std::vector<std::string> getPaths (const std::string& categoryTag, long limit, long offset) override;

    leveldb::DB* getDatabase (const std::string& categoryTag);

private:
    LevelDbFactory* m_levelDbFactory;
    std::unordered_map<std::string, leveldb::DB*> m_categoryDBStore;
};

} // namespace filesaver::services

#endif // FILESAVER_LEVELDBFILECATEGORYSTORE_H
