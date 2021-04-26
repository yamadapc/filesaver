//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include "FileSizePair.h"

#include <utility>

namespace filesaver
{

FileSizePair::FileSizePair (std::string _filename, off_t _size, std::shared_ptr<FileEntry> entry)
    : filename (std::move (_filename)), size (_size), m_entry (std::move(entry))
{
}

const std::string& FileSizePair::getFilename () const
{
    return filename;
}

off_t FileSizePair::getSize () const
{
    return size;
}

std::shared_ptr<FileEntry> FileSizePair::getEntry () const
{
    return m_entry;
}

} // namespace filesaver
