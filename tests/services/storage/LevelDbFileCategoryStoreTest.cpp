//
// Created by Pedro Tacla Yamada on 28/4/21.
//

#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <lfilesaver/services/category/LevelDbFileCategoryStore.h>
#include <lfilesaver/services/settings/SettingsService.h>
#include <lfilesaver/services/storage/LevelDbFactory.h>

TEST_CASE ("LevelDbFileCategoryStore can add and list paths on a category")
{
    using namespace filesaver;
    using namespace filesaver::services;
    using namespace filesaver::services::settings;

    boost::filesystem::remove_all ("/tmp/filesaver-settings");
    boost::filesystem::create_directories ("/tmp/filesaver-settings");
    SettingsService settingsService{"/tmp/filesaver-settings/settings.yml"};
    LevelDbFactory factory{&settingsService};
    LevelDbFileCategoryStore store{&factory};

    store.insertPath ("category1", "path1");
    store.insertPath ("category1", "path2");
    store.insertPath ("category2", "path3");
    store.insertPath ("category2", "path4");

    auto result1 = store.getPaths ("category1", 2, 0);
    REQUIRE (result1.size () == 2);
    INFO (result1[0]);
    INFO (result1[1]);
    REQUIRE (result1[0] == "path1");
    REQUIRE (result1[1] == "path2");
    auto result2 = store.getPaths ("category2", 2, 0);
    INFO (result2[0]);
    INFO (result2[1]);
    REQUIRE (result2.size () == 2);
    REQUIRE (result2[0] == "path3");
    REQUIRE (result2[1] == "path4");
}

TEST_CASE ("LevelDbFileCategoryStore can read with an offset & limit")
{
    using namespace filesaver;
    using namespace filesaver::services;
    using namespace filesaver::services::settings;

    boost::filesystem::remove_all ("/tmp/filesaver-settings");
    boost::filesystem::create_directories ("/tmp/filesaver-settings");
    SettingsService settingsService{"/tmp/filesaver-settings/settings.yml"};
    LevelDbFactory factory{&settingsService};
    LevelDbFileCategoryStore store{&factory};

    store.insertPath ("category1", "path1");
    store.insertPath ("category1", "path2");
    store.insertPath ("category1", "path3");

    auto result1 = store.getPaths ("category1", 1, 0);
    REQUIRE (result1.size () == 1);
    REQUIRE (result1[0] == "path1");
    auto result2 = store.getPaths ("category1", 4, 1);
    REQUIRE (result2.size () == 2);
    REQUIRE (result2[0] == "path2");
    REQUIRE (result2[1] == "path3");
}

TEST_CASE ("LevelDbFileCategoryStore does not insert duplicates")
{
    using namespace filesaver;
    using namespace filesaver::services;
    using namespace filesaver::services::settings;

    boost::filesystem::remove_all ("/tmp/filesaver-settings");
    boost::filesystem::create_directories ("/tmp/filesaver-settings");
    SettingsService settingsService{"/tmp/filesaver-settings/settings.yml"};
    LevelDbFactory factory{&settingsService};
    LevelDbFileCategoryStore store{&factory};
    auto result0 = store.getPaths ("category1", 10, 0);
    REQUIRE (result0.size () == 0);

    store.insertPath ("category1", "path1");
    store.insertPath ("category1", "path1");
    store.insertPath ("category1", "path1");

    auto result1 = store.getPaths ("category1", 10, 0);
    REQUIRE (result1.size () == 1);
    REQUIRE (result1[0] == "path1");
}
