//
// Created by Pedro Tacla Yamada on 29/4/21.
//

#include <boost/filesystem/path.hpp>
#include <spdlog/spdlog.h>

#include "CompressionError.h"
#include "CompressionService.h"

namespace filesaver::services::fileutils
{

void CompressionService::compressDirectory (const std::string& target, const std::string& destination)
{
    boost::filesystem::path targetPath{target};
    auto targetFilename = targetPath.filename ();
    auto targetDirectory = targetPath.remove_filename ();

    auto previousPath = boost::filesystem::current_path ();
    boost::filesystem::current_path (targetDirectory.string ());

    spdlog::info ("Compressing directory target={} destination={} targetFilename={} targetDirectory={}",
                  target,
                  destination,
                  targetFilename.string (),
                  targetDirectory.string ());

    int result = boost::process::system (fmt::format ("zip -r {} {}", destination, targetFilename.string ()));
    boost::filesystem::current_path (previousPath);

    if (result != 0)
    {
        throw CompressionError{};
    }
}

void CompressionService::decompressDirectory (const std::string& destination)
{
    auto previousPath = boost::filesystem::current_path ();
    boost::filesystem::path destinationPath{destination};
    boost::filesystem::current_path (destinationPath.remove_filename ());

    spdlog::info ("Uncompressing directory {}", destination);
    boost::process::ipstream inputProcessStream;
    int result = boost::process::system (fmt::format ("unzip {}", destination));
    boost::filesystem::current_path (previousPath);
    if (result != 0)
    {
        throw CompressionError{};
    }
}

} // namespace filesaver::services::fileutils
