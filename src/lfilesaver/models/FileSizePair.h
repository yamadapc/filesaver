//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILESAVER_FILESIZEPAIR_H
#define FILESAVER_FILESIZEPAIR_H

#include <string>

#include "FileEntry.h"

namespace filesaver
{

/// Wrap filename and size
class FileSizePair
{
public:
    FileSizePair (std::string _filename, off_t _size, std::shared_ptr<FileEntry> entry);

    const std::string& getFilename () const;
    off_t getSize () const;
    std::shared_ptr<FileEntry> getEntry () const;

private:
    std::string filename;
    off_t size;
    std::shared_ptr<FileEntry> m_entry;
};

} // namespace filesaver

#endif // FILESAVER_FILESIZEPAIR_H
