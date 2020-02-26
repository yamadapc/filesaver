//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include <boost/filesystem.hpp>
#include <iostream>

#include "gtest/gtest.h"

#include "../../src/models/FileEntry.h"

TEST (FileEntryTest, CanTellTheSizeOfFiles)
{
    auto tmp = boost::filesystem::temp_directory_path ();
    tmp.append ("test-file.txt");
    {
        boost::filesystem::ofstream outputStream (tmp);
        outputStream << "some stuff here";
    }

    EXPECT_TRUE (boost::filesystem::exists (tmp));
    auto entry = filesaver::FileEntry::fromPath (tmp.string ());
    auto size = entry->size;

    EXPECT_GT (size, 0);
    EXPECT_EQ (size, boost::filesystem::file_size (tmp));
}

TEST (FileEntryTest, CanTellTheSizeOfDirectories)
{
    auto tmp = boost::filesystem::temp_directory_path ();
    tmp.append ("test-directory");
    boost::filesystem::create_directory (tmp);
    EXPECT_TRUE (boost::filesystem::exists (tmp));

    auto entry = filesaver::FileEntry::fromPath (tmp.string ());
    auto size = entry->size;

    EXPECT_GT (size, 0);
    EXPECT_EQ (entry->isDirectory (), true);
}

TEST (FileEntryTest, CanListDirectoryChildren)
{
    auto tmp = boost::filesystem::temp_directory_path ();
    tmp.append ("test-directory");
    boost::filesystem::create_directory (tmp);
    EXPECT_TRUE (boost::filesystem::exists (tmp));

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

    EXPECT_EQ (children.size (), 2);

    {
        auto it = std::find (children.begin (), children.end (), tmpFile1.string ());
        EXPECT_NE (it, children.end ());
        EXPECT_EQ (it->string (), tmpFile1.string ());
    }

    {
        auto it = std::find (children.begin (), children.end (), tmpFile2.string ());
        EXPECT_NE (it, children.end ());
        EXPECT_EQ (it->string (), tmpFile2.string ());
    }
}
