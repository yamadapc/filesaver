//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include "FileSizePair.h"

namespace filesaver
{

FileSizePair::FileSizePair (std::string filename, off_t size) : filename (std::move (filename)), size (size)
{
}

const std::string& FileSizePair::getFilename ()
{
    return filename;
}

off_t FileSizePair::getSize ()
{
    return size;
}

} // namespace filesaver
