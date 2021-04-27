//
// Created by Pedro Tacla Yamada on 27/4/21.
//

#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <lfilesaver/models/FileEntry.h>
#include <lfilesaver/services/settings/SettingsService.h>
#include <lfilesaver/services/storage/LevelDbFactory.h>
#include <unordered_set>

TEST_CASE ("LevelDbFactory - can return DB paths in the settings path")
{
    using namespace filesaver;
    using namespace filesaver::services;
    using namespace filesaver::services::settings;

    boost::filesystem::remove_all ("/tmp/filesaver-settings");
    boost::filesystem::create_directories ("/tmp/filesaver-settings");
    SettingsService settingsService{"/tmp/filesaver-settings/settings.yml"};
    LevelDbFactory factory{&settingsService};

    leveldb::Options options;
    options.create_if_missing = true;

    std::string databaseTag = "test-db-1234";
    leveldb::DB* databasePtr = nullptr;
    auto dbPath = factory.getPathForTag (databaseTag);
    REQUIRE (dbPath == "/tmp/filesaver-settings/test-db-1234.db");
    auto status = factory.openDatabase (options, databaseTag, &databasePtr);

    INFO (fmt::format ("DB open: {}", status.ToString ()));
    REQUIRE (status.ok ());
    REQUIRE (databasePtr != nullptr);
    databasePtr->Put ({}, "test-key", "test-value");
    std::string value;
    status = databasePtr->Get ({}, "test-key", &value);

    INFO (fmt::format ("DB get: {}", status.ToString ()));
    REQUIRE (status.ok ());
    REQUIRE (value == "test-value");

    auto fileEntry = FileEntry::fromPath ("/tmp/filesaver-settings/");
    const auto& childrenVector = fileEntry->children ();
    REQUIRE (childrenVector.size () == 1);
    std::vector<std::string> childrenVectorStrs{childrenVector.size ()};
    std::transform (childrenVector.begin (),
                    childrenVector.end (),
                    childrenVectorStrs.begin (),
                    [] (boost::filesystem::path path) -> std::string { return path.filename ().string (); });
    std::unordered_set<std::string> children{childrenVectorStrs.begin (), childrenVectorStrs.end ()};

    for (auto c : childrenVectorStrs)
    {
        INFO (fmt::format ("Child directory {}", c));
    }

    REQUIRE (children.find ("test-db-1234.db") != children.end ());

    INFO ("Cleaning-up")
    boost::filesystem::remove_all ("/tmp/filesaver-settings");
}
