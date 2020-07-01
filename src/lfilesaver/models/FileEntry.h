//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#ifndef FILE_SAVER_FILEENTRY_H
#define FILE_SAVER_FILEENTRY_H

#include <boost/filesystem/path.hpp>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <utility>
#include <vector>

#include "FileSizePair.h"

namespace filesaver
{

/// File type enum
enum class FileType
{
    directory = 0,
    regular_file = 1,
    symbolic_link = 2,
    socket = 3,
    block_special = 4,
    character_special = 5,
    fifo_special = 6,
    unknown = 7,
};

/// File entry abstraction
/**
 * Wraps a file entry in an object, provide utility functions for reading this
 * information from a path and cache child files in memory.
 */
class FileEntry
{
public:
    uintmax_t dev = 0;
    uintmax_t ino = 0;

    FileType type = FileType::unknown;
    off_t size = 0;
    boost::filesystem::path filepath;
    bool isFinished = false;

    FileEntry () = default;
    FileEntry (FileType _type, off_t _size, uintmax_t _dev, uintmax_t _ino, std::string _filename);
    FileEntry (FileType _type, off_t _size, uintmax_t _dev, uintmax_t _ino, boost::filesystem::path _filepath);

    bool operator== (const FileEntry& rhs) const;
    bool operator!= (const FileEntry& rhs) const;

    [[nodiscard]] bool isDirectory () const;
    [[nodiscard]] bool getHasCachedChildren () const;
    void setCachedChildren (std::vector<boost::filesystem::path> cachedChildren);
    const std::vector<boost::filesystem::path>& children ();

    [[nodiscard]] FileSizePair toPair () const;

    static std::shared_ptr<FileEntry> fromPath (const boost::filesystem::path& filepath);

private:
    std::vector<boost::filesystem::path> cachedChildren{};
    bool hasCachedChildren = false;
};

} // namespace filesaver

#endif // FILE_SAVER_FILEENTRY_H
