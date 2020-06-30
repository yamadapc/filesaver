//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include "FileSizePair.h"

namespace filesaver
{

FileSizePair::FileSizePair (std::string _filename, off_t _size) : filename (std::move (_filename)), size (_size)
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

} // namespace filesaver
