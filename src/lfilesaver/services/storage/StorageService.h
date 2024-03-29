//
// Created by Pedro Tacla Yamada on 2019-09-02.
//

#ifndef FILE_SAVER_STORAGESERVICE_H
#define FILE_SAVER_STORAGESERVICE_H

#include <cstddef>
#include <optional>
#include <vector>

#include "../../models/FileSizePair.h"

namespace filesaver
{

/// Interface for storage back-ends
class StorageService
{
public:
    virtual ~StorageService () = default;

    /**
     * Perform setup for this storage back-end
     */
    virtual int createTables () = 0;

    /**
     * Insert an entry onto the store
     */
    virtual int insertEntry (const FileSizePair& pair) = 0;
    virtual int insertEntryBatch (const std::vector<FileSizePair>& pairs, size_t start, size_t end) = 0;

    /**
     * Fetch an entry from the store
     */
    virtual std::optional<FileSizePair> fetchEntry (const std::string& filepath) = 0;
};

} // namespace filesaver

#endif // FILE_SAVER_STORAGESERVICE_H
