//
// Created by Pedro Tacla Yamada on 4/7/20.
//

#pragma once

#include <fruit/fruit.h>
#include <memory>
#include <mutex>
#include <optional>

#include "../../models/FileEntry.h"
#include "./InMemoryFileEntryStore.h"

namespace filesaver::services
{

class InMemoryFileSizeService
{
public:
    INJECT (InMemoryFileSizeService ());

    std::optional<off_t> getCurrentSizeAt (const std::string& filepath);
    void onFileEntry (std::shared_ptr<FileEntry> entry);
    void onFileEntryBulk (std::vector<std::shared_ptr<FileEntry>> entries);
    void cleanEntry (std::string key);
    void cleanEntryBulk (std::vector<std::string> keys);
    void setDelegate (InMemoryFileEntryStore::Delegate* delegate);
    void clearDelegate ();

private:
    std::mutex m_inMemoryStoreMutex;
    InMemoryFileEntryStore m_inMemoryStore;
};

} // namespace filesaver::services
