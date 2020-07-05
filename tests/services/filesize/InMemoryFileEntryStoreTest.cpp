//
// Created by Pedro Tacla Yamada on 2/7/20.
//

#include <catch2/catch.hpp>
#include <memory>
#include <string>

#include <lfilesaver/models/FileEntry.h>
#include <lfilesaver/services/filesize/InMemoryFileEntryStore.h>

void requireEmpty (filesaver::services::InMemoryFileEntryStore& store)
{
    REQUIRE (store.getDelegate () == nullptr);
    REQUIRE (store.getHashMapSize () == 0);
    REQUIRE (!store.getCurrentSizeAt ("test").has_value ());
    REQUIRE (!store.getCurrentSizeAt ("/").has_value ());
}

TEST_CASE ("InMemoryFileEntryStore empty state makes sense")
{
    using namespace filesaver::services;
    InMemoryFileEntryStore store;
    requireEmpty (store);
}

TEST_CASE ("InMemoryFileEntryStore we can add file entries to the store")
{
    using namespace filesaver::services;
    using namespace filesaver;

    InMemoryFileEntryStore store;
    requireEmpty (store);
    auto entry = std::make_shared<FileEntry> (FileType::regular_file, 10, 0, 0, std::string{"/dir/file"});
    store.addEntry (entry);
    REQUIRE (store.getHashMapSize () == 1);
    REQUIRE (store.getCurrentSizeAt ("/dir/file").has_value ());
    REQUIRE (store.getCurrentSizeAt ("/dir/file").value () == 10);
    REQUIRE (store.isPathFinished ("/dir/file"));

    // Directories need to be inserted before their children
    REQUIRE (!store.getCurrentSizeAt ("/dir").has_value ());
    REQUIRE (!store.getCurrentSizeAt ("/").has_value ());
}

TEST_CASE ("InMemoryFileEntryStore adding files increment their parent sizes")
{
    using namespace filesaver::services;
    using namespace filesaver;

    InMemoryFileEntryStore store;
    requireEmpty (store);
    auto parent = std::make_shared<FileEntry> (FileType::directory, 10, 0, 0, std::string{"/dir"});
    parent->setCachedChildren ({{"/dir/file"}});
    auto entry = std::make_shared<FileEntry> (FileType::regular_file, 10, 0, 0, std::string{"/dir/file"});

    store.addEntry (parent);
    store.addEntry (entry);

    REQUIRE (store.getHashMapSize () == 2);
    REQUIRE (store.getCurrentSizeAt ("/dir/file").has_value ());
    REQUIRE (store.getCurrentSizeAt ("/dir/file").value () == 10);
    REQUIRE (store.isPathFinished ("/dir/file"));

    REQUIRE (store.getCurrentSizeAt ("/dir").has_value ());
    REQUIRE (store.getCurrentSizeAt ("/dir").value () == 20);
    REQUIRE (store.isPathFinished ("/dir"));
}

TEST_CASE ("InMemoryFileEntryStore parents aren't finished before all their children")
{
    using namespace filesaver::services;
    using namespace filesaver;

    InMemoryFileEntryStore store;
    requireEmpty (store);
    auto parent = std::make_shared<FileEntry> (FileType::directory, 10, 0, 0, std::string{"/dir"});
    parent->setCachedChildren (std::vector<boost::filesystem::path>{{"/dir/file"}, {"/dir/other_file"}});
    auto entry = std::make_shared<FileEntry> (FileType::regular_file, 10, 0, 0, std::string{"/dir/file"});

    store.addEntry (parent);
    store.addEntry (entry);

    REQUIRE (!store.isPathFinished ("/dir"));
}

TEST_CASE ("InMemoryFileEntryStore clean-up gets us back to empty states")
{
    using namespace filesaver::services;
    using namespace filesaver;

    InMemoryFileEntryStore store;
    requireEmpty (store);
    auto parent = std::make_shared<FileEntry> (FileType::directory, 10, 0, 0, std::string{"/dir"});
    parent->setCachedChildren (std::vector<boost::filesystem::path>{{"/dir/file"}, {"/dir/other_file"}});
    auto entry = std::make_shared<FileEntry> (FileType::regular_file, 10, 0, 0, std::string{"/dir/file"});

    store.addEntry (parent);
    store.addEntry (entry);

    REQUIRE (store.getHashMapSize () == 2);
    store.cleanEntry (parent->filepath.string ());
    REQUIRE (store.getHashMapSize () == 1);
    store.cleanEntry (entry->filepath.string ());
    requireEmpty (store);
}

TEST_CASE ("InMemoryFileEntryStore can set a delegate for entry finished")
{
    using namespace filesaver::services;
    using namespace filesaver;

    class MockDelegate : public virtual InMemoryFileEntryStore::Delegate
    {
    public:
        MockDelegate () = default;

        void onPathFinished (InMemoryFileEntryStore::Record) override
        {
        }
    };

    MockDelegate mockDelegate;
    InMemoryFileEntryStore store;
    requireEmpty (store);
    store.setDelegate (&mockDelegate);
    REQUIRE (store.getDelegate () == &mockDelegate);
    store.clearDelegate ();
    REQUIRE (store.getDelegate () == nullptr);
}
