//
// Created by Pedro Tacla Yamada on 23/5/20.
//

#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <chrono>
#include <string>

#include <lfilesaver/services/storage/LevelDbStorageService.h>

TEST_CASE ("LevelDbStorageService - can be constructed with a file-name")
{
    using filesaver::LevelDbStorageService;
    auto dirname = boost::filesystem::temp_directory_path ();
    auto testDir = dirname;
    testDir.append ("__test__").append ("constructor");
    auto dbPath = testDir;
    dbPath.append ("test.db");

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
    auto testDir = dirname;
    testDir.append ("__test__").append ("io");
    auto dbPath = testDir;
    dbPath.append ("test.db");

    boost::filesystem::remove_all (dbPath);
    boost::filesystem::create_directories (testDir);
    LevelDbStorageService storageService{dbPath.string ()};
    REQUIRE (storageService.isDatabaseOk ());
    REQUIRE (boost::filesystem::exists (dbPath));
    REQUIRE (storageService.createTables () == 0);

    SECTION ("inserting entry outputs good status")
    {
        filesaver::FileSizePair testEntry{"/something_here", 0L};
        REQUIRE (storageService.insertEntry (testEntry));
    }

    SECTION ("fetching an unknown entry")
    {
        auto result = storageService.fetchEntry ("/unknown");
        REQUIRE (!result.has_value ());
    }

    SECTION ("when inserted an entry can be fetched")
    {
        std::optional<int> o = {};
        REQUIRE (!o.has_value ());

        filesaver::FileSizePair testEntry{"/other", 100U};
        REQUIRE (storageService.insertEntry (testEntry));
        auto result = storageService.fetchEntry ("/other");
        REQUIRE (result.has_value ());
        REQUIRE (result.value ().getSize () == 100);
        REQUIRE (result.value ().getFilename () == "/other");
    }

    boost::filesystem::remove_all (testDir);
}
