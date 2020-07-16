//
// Created by Pedro Tacla Yamada on 13/7/20.
//

#include <boost/filesystem/operations.hpp>

#include "RootPathProvider.h"

namespace filesaver::services
{

boost::filesystem::path DefaultRootPathProvider::getFileSaverRoot ()
{
    boost::filesystem::path root{getenv ("HOME")};
    root.append (".filesaver");
    return boost::filesystem::canonical (root);
}

} // namespace filesaver::services
