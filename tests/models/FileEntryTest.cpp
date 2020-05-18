//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <iostream>

#include <lfilesaver/models/FileEntry.h>

TEST_CASE ("FileEntryTest - CanTellTheSizeOfFiles")
{
    auto tmp = boost::filesystem::temp_directory_path ();
    tmp.append ("test-file.txt");
    {
        boost::filesystem::ofstream outputStream (tmp);
        outputStream << "some stuff here";
    }

    REQUIRE (boost::filesystem::exists (tmp));
    auto entry = filesaver::FileEntry::fromPath (tmp.string ());
    auto size = entry->size;

    REQUIRE (size > 0);
    REQUIRE (static_cast<unsigned long> (size) == boost::filesystem::file_size (tmp));
}

TEST_CASE ("FileEntryTest - CanTellTheSizeOfDirectories")
{
    auto tmp = boost::filesystem::temp_directory_path ();
    tmp.append ("test-directory");
    boost::filesystem::create_directory (tmp);
    REQUIRE (boost::filesystem::exists (tmp));

    auto entry = filesaver::FileEntry::fromPath (tmp.string ());
    auto size = entry->size;

    REQUIRE (size > 0);
    REQUIRE (entry->isDirectory ());
}

TEST_CASE ("FileEntryTest - CanListDirectoryChildren")
{
    auto tmp = boost::filesystem::temp_directory_path ();
    tmp.append ("test-directory");
    boost::filesystem::create_directory (tmp);
    REQUIRE (boost::filesystem::exists (tmp));

    auto tmpFile1 = tmp;
    tmpFile1.append ("file1.txt");
    auto tmpFile2 = tmp;
    tmpFile2.append ("file2.txt");

    {
        boost::filesystem::ofstream outputStream (tmpFile1);
        outputStream << "some stuff here 123";
    }
    {
        boost::filesystem::ofstream outputStream (tmpFile2);
        outputStream << "some stuff here 123";
    }

    auto entry = filesaver::FileEntry::fromPath (tmp.string ());
    auto children = entry->children ();

    REQUIRE (children.size () == 2);

    {
        auto it = std::find (children.begin (), children.end (), tmpFile1.string ());
        REQUIRE (it != children.end ());
        REQUIRE (it->string () == tmpFile1.string ());
    }

    {
        auto it = std::find (children.begin (), children.end (), tmpFile2.string ());
        REQUIRE (it != children.end ());
        REQUIRE (it->string () == tmpFile2.string ());
    }
}
