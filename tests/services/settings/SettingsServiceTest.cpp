//
// Created by Pedro Tacla Yamada on 24/5/20.
//

#include <catch2/catch.hpp>

#include <lfilesaver/services/settings/SettingsService.h>

#include "../../test_utils/TestUtils.h"

TEST_CASE ("SettingsService - we can load settings if there're none")
{
    using filesaver::services::settings::SettingsService;
    using namespace filesaver::test;

    auto mockPath = getRandomTemporaryPath ();
    SettingsService settingsService{mockPath};

    settingsService.loadSettings ();
}

TEST_CASE ("SettingsService - we can load settings")
{
    using filesaver::services::settings::SettingsService;

    boost::filesystem::path file{__FILE__};
    auto mockPath = file.parent_path ().append ("__mocks__").append ("test.yaml");
    SettingsService settingsService{mockPath};
    REQUIRE (settingsService.loadSettings ());

    auto result = settingsService.get<std::string> ("setting");
    REQUIRE (result.has_value ());
    REQUIRE (result.value () == "here");
}

TEST_CASE ("SettingsService - we can store settings")
{
    using filesaver::services::settings::SettingsService;
    boost::filesystem::path file{__FILE__};
    auto mockPath = file.parent_path ().append ("__mocks__").append ("test-load-store.yaml");
    SettingsService settingsService{mockPath};

    SECTION ("Storing")
    {
        boost::filesystem::remove (mockPath);
        settingsService.set ("setting", "here");
        settingsService.saveSettings ();
    }

    SECTION ("Loading")
    {
        REQUIRE (settingsService.loadSettings ());

        auto result = settingsService.get<std::string> ("setting");
        REQUIRE (result.has_value ());
        REQUIRE (result.value () == "here");
    }
}
