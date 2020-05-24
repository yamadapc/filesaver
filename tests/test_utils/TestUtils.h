//
// Created by Pedro Tacla Yamada on 24/5/20.
//

#pragma once

#include <boost/filesystem.hpp>
#include <random>

namespace filesaver::test
{

std::string randomString (int size);

boost::filesystem::path getRandomTemporaryPath ();

} // namespace filesaver::test
