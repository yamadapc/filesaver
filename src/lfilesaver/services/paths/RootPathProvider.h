//
// Created by Pedro Tacla Yamada on 13/7/20.
//


#pragma once

#include <boost/filesystem/path.hpp>

namespace filesaver::services
{

class RootPathProvider
{
public:
    virtual ~RootPathProvider () = default;
    virtual boost::filesystem::path getFileSaverRoot () = 0;
};

class DefaultRootPathProvider final : RootPathProvider
{
    boost::filesystem::path getFileSaverRoot () override;
};

} // namespace filesaver::services
