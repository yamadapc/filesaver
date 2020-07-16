//
// Created by Pedro Tacla Yamada on 11/7/20.
//

#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>

#include "../test_utils/RandomRootGenerator.h"
#include "../test_utils/TestUtils.h"

TEST_CASE ("Generating mock roots work")
{
    using namespace filesaver::test;

    auto tmpPath = getRandomTemporaryPath ();
    spdlog::info ("Generating root at {}", tmpPath.string ());
    RandomRootGenerator::generateRootAt (tmpPath);
}