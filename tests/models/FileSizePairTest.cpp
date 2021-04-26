//
// Created by Pedro Tacla Yamada on 23/5/20.
//

#include <catch2/catch.hpp>

#include <lfilesaver/models/FileSizePair.h>

TEST_CASE ("FileSizePair - can build objects")
{
    using filesaver::FileSizePair;
    FileSizePair pair{"filename", 10, nullptr};

    REQUIRE (pair.getFilename () == "filename");
    REQUIRE (pair.getSize () == 10);
}
