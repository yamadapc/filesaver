//
// Created by Pedro Tacla Yamada on 11/7/20.
//


#pragma once

#include <boost/filesystem.hpp>

namespace filesaver::test
{

class RandomRootGenerator
{
public:
    static void generateRootAt (boost::filesystem::path path);

private:
};

} // namespace filesaver::test
