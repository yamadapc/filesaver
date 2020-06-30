//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILESAVER_FILESIZEPAIR_H
#define FILESAVER_FILESIZEPAIR_H

#include <string>
#include <utility>

namespace filesaver
{

/// Wrap filename and size
class FileSizePair
{
public:
    FileSizePair (std::string _filename, off_t _size);

    const std::string& getFilename () const;
    off_t getSize () const;

private:
    std::string filename;
    off_t size;
};

} // namespace filesaver

#endif // FILESAVER_FILESIZEPAIR_H
