//
// Created by Pedro Tacla Yamada on 23/5/20.
//

#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <string>

#include <lfilesaver/services/storage/LevelDbStorageService.h>

TEST_CASE ("LevelDbStorageService - can be constructed with a file-name")
{
    using filesaver::LevelDbStorageService;
    auto dirname = boost::filesystem::temp_directory_path ();
    auto testDir = dirname.append ("__test__").append ("constructor");
    auto dbPath = testDir.append ("test.db");

    boost::filesystem::remove_all (dbPath);
    boost::filesystem::create_directories (testDir);

    SECTION ("simple constructor invocation")
    {
        LevelDbStorageService storageService{dbPath.string ()};
        REQUIRE (boost::filesystem::exists (dbPath));
    }

    SECTION ("Set-up returns 0")
    {
        LevelDbStorageService storageService{dbPath.string ()};
        REQUIRE (storageService.createTables () == 0);
    }

    boost::filesystem::remove_all (testDir);
}

TEST_CASE ("LevelDbStorageService - inserting/retrieving entries")
{
    using filesaver::FileEntry;
    using filesaver::LevelDbStorageService;
    auto dirname = boost::filesystem::temp_directory_path ();
    auto testDir = dirname.append ("__test__").append ("io");
    auto dbPath = testDir.append ("test.db");

    boost::filesystem::remove_all (dbPath);
    boost::filesystem::create_directories (testDir);
    LevelDbStorageService storageService{dbPath.string ()};
    REQUIRE (boost::filesystem::exists (dbPath));
    REQUIRE (storageService.createTables () == 0);

    SECTION ("inserting entry outputs good status")
    {
        FileEntry testEntry;
        testEntry.filepath = "/something_here";
        REQUIRE (storageService.insertEntry (testEntry));
    }

    SECTION ("when inserted an entry can be fetched")
    {
        FileEntry testEntry;
        testEntry.filepath = "/something_here";
        testEntry.size = 10;
        REQUIRE (storageService.insertEntry (testEntry));
        auto result = storageService.fetchEntry ("/something_here");
        REQUIRE (result.has_value ());
        REQUIRE (result.value ().getSize () == 10);
        REQUIRE (result.value ().getFilename () == "/something_here");
    }

    boost::filesystem::remove_all (testDir);
}
