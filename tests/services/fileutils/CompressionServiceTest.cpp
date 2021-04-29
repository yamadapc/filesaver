//
// Created by Pedro Tacla Yamada on 29/4/21.
//

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <lfilesaver/services/fileutils/CompressionService.h>

#include "../../test_utils/RandomRootGenerator.h"
#include "../../test_utils/TestUtils.h"

TEST_CASE ("CompressionService::compressDirectory")
{
    using namespace filesaver;
    using namespace filesaver::services;
    using namespace filesaver::services::fileutils;
    using namespace filesaver::test;

    CompressionService compressionService;
    auto temporaryPath = getRandomTemporaryPath ();
    RandomRootGenerator::generateRootAt (temporaryPath);

    auto zipFilePathStr = fmt::format ("{}.zip", temporaryPath.string ());
    compressionService.compressDirectory (temporaryPath.string (), zipFilePathStr);

    boost::filesystem::path zipFilePath{zipFilePathStr};
    REQUIRE (boost::filesystem::exists (zipFilePath));

    boost::filesystem::remove_all (temporaryPath);
    REQUIRE (!boost::filesystem::exists (temporaryPath));

    compressionService.decompressDirectory (zipFilePathStr);
    REQUIRE (boost::filesystem::exists (temporaryPath));
    boost::filesystem::remove_all (temporaryPath);
}