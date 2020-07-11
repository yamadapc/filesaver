//
// Created by Pedro Tacla Yamada on 23/5/20.
//

#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <chrono>
#include <fmt/format.h>
#include <memory>
#include <thread>

#include <lfilesaver/data/WorkQueue.h>
#include <lfilesaver/models/FileEntry.h>
#include <lfilesaver/workers/Worker.h>

TEST_CASE ("Worker")
{
    using boost::filesystem::path;
    using filesaver::FileEntry;
    using filesaver::Worker;
    using filesaver::data::WorkQueue;
    using std::shared_ptr;

    auto workQueue = std::make_shared<WorkQueue<path>> ();
    auto resultQueue = std::make_shared<WorkQueue<shared_ptr<FileEntry>>> ();

    Worker worker{workQueue, resultQueue};
    std::thread workerThread{&Worker::start, &worker};

    SECTION ("We can start and stop")
    {
        // Just falling through to check we can call stop and join and exit
    }

    SECTION ("If a filepath is submitted to the work queue, we'll get its entry back")
    {
        auto tmpDir = boost::filesystem::temp_directory_path ();
        auto randomFilePath = tmpDir;
        randomFilePath.append ("random-file");
        {
            std::ofstream outputStream{randomFilePath.string ()};
            outputStream << "hello";
        }

        workQueue->push (randomFilePath);
        auto result = resultQueue->frontWithTimeout (std::chrono::milliseconds (100));
        REQUIRE (result.has_value ());

        auto entry = result.value ();
        REQUIRE (entry->filepath == randomFilePath.string ());
        REQUIRE (entry->size == 5);
        REQUIRE (entry->type == filesaver::FileType::regular_file);
    }

    // Wait until the worker has really started
    std::this_thread::sleep_for (std::chrono::milliseconds (10));
    worker.stop ();
    workerThread.join ();
}

TEST_CASE ("Worker::processEntry")
{
    using boost::filesystem::path;
    using filesaver::FileEntry;
    using filesaver::Worker;
    using filesaver::data::WorkQueue;
    using std::shared_ptr;

    auto workQueue = std::make_shared<WorkQueue<path>> ();
    auto resultQueue = std::make_shared<WorkQueue<shared_ptr<FileEntry>>> ();

    Worker worker{workQueue, resultQueue};

    SECTION ("If a directory is submitted we'll get its children pushed to the workQueue")
    {
        auto tmpDir = boost::filesystem::temp_directory_path ().append (
            fmt::format ("filesaver__worker_test__{}", std::chrono::system_clock::now ().time_since_epoch ().count ()));

        boost::filesystem::remove_all (tmpDir);
        boost::filesystem::create_directories (tmpDir);

        auto randomFilePath1 = tmpDir;
        randomFilePath1.append ("random-file1");
        {
            std::ofstream outputStream{randomFilePath1.string ()};
            outputStream << "hello";
        }
        auto randomFilePath2 = tmpDir;
        randomFilePath2.append ("random-file2");
        {
            std::ofstream outputStream{randomFilePath2.string ()};
            outputStream << "hello";
        }

        INFO ("Temporary directory = " << tmpDir.string ());
        REQUIRE (workQueue->size () == 0);
        REQUIRE (resultQueue->size () == 0);

        worker.processEntry (tmpDir);

        REQUIRE (resultQueue->size () == 1);
        REQUIRE (workQueue->size () == 2);

        boost::filesystem::remove_all (tmpDir);
    }
}
