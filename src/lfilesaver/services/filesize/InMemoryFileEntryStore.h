//
// Created by Pedro Tacla Yamada on 30/6/20.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/optional/optional.hpp>
#include <optional>
#include <unordered_map>

#include "../../models/FileEntry.h"

namespace filesaver::services
{

class InMemoryFileEntryStore
{
public:
    struct Record
    {
        std::shared_ptr<FileEntry> fileEntry = nullptr;
        off_t totalSize = 0;
        unsigned long pendingChildren = 0;
    };

    class Delegate
    {
    public:
        virtual ~Delegate ()
        {
        }

        virtual void onPathFinished (Record& inMemoryRecord) = 0;
    };

    InMemoryFileEntryStore ();
    explicit InMemoryFileEntryStore (Delegate* storeDelegate);

    /// Push an entry onto the in-memory store
    void addEntry (std::shared_ptr<FileEntry> entry);

    /// Clean entry from memory
    void cleanEntry (const std::string& filepath);

    /// Get the current in-memory size for a path
    std::optional<off_t> getCurrentSizeAt (const std::string& filepath);

    /// Return true if this path has been finished
    bool isPathFinished (const std::string& filepath);

    /// Returns the size of the backing hash-map
    size_t getHashMapSize ();

    Delegate* getDelegate ();
    void setDelegate (Delegate* delegate);
    void clearDelegate ();

private:
    /// When an entry is pushed, update its finished state and all of its parents
    void updatePendingAndFinishedState (boost::filesystem::path filepath);

    /// When an entry is pushed, update its size and all of its parents
    void updateSizes (const std::shared_ptr<filesaver::FileEntry>& entry);

    /// Adds a size delta recursively to this path and its parents
    void addSizeRecursive (const boost::filesystem::path& filepath, off_t sizeDiff);

    /// Adds a size delta to this path and its parents
    void addSize (const boost::filesystem::path& path, off_t sizeDiff);

    std::unordered_map<std::string, Record> m_records{};
    boost::optional<Delegate*> m_delegate{};
};

} // namespace filesaver::services
