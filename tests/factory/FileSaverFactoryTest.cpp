//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include <catch2/catch.hpp>
#include <fruit/fruit.h>
#include <memory>

#include "../../src/lfilesaver/factory/FileSaverFactory.h"

TEST_CASE ("FileSaverFactory can build a filesaver instance")
{
    filesaver::FileSaverFactory factory;
    auto& filesaver = factory.getRef ();
    REQUIRE (!filesaver.getVersion ().empty ());
}

TEST_CASE ("FileSaverFactory will always build the same instance")
{
    filesaver::FileSaverFactory factory;
    auto* filesaver1 = factory.getPtr ();
    auto* filesaver2 = factory.getPtr ();
    REQUIRE (filesaver1 == filesaver2);
}
