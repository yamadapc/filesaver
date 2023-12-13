//
// Created by Pedro Tacla Yamada on 29/4/21.
//

#ifndef FILESAVER_COMPRESSIONSERVICE_H
#define FILESAVER_COMPRESSIONSERVICE_H

#include <boost/process.hpp>

namespace filesaver::services::fileutils
{

class CompressionService
{
public:
    CompressionService () = default;

    static void compressDirectory (const std::string& target, const std::string& destination);
    static void decompressDirectory (const std::string& destination);
};

} // namespace filesaver::services::fileutils

#endif // FILESAVER_COMPRESSIONSERVICE_H
