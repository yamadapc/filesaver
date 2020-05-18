//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "CommandLineApp.h"

namespace filesaver
{

namespace po = boost::program_options;

int CommandLineApp::main (int argc, char** argv)
{
    po::options_description publicDescription ("General options");
    po::variables_map variablesMap;
    po::positional_options_description trailingFilesDescription;
    std::vector<std::string> inputFiles;
    unsigned int numWorkers = 0;

    publicDescription.add_options () ("help,h", "print this help message") ("storage", "create leveldb index") (
        "num-workers", po::value (&numWorkers), "The number of worker threads to use") (
        "input-file", po::value (&inputFiles), "input file");

    trailingFilesDescription.add ("input-file", -1);

    po::store (
        po::command_line_parser (argc, argv).options (publicDescription).positional (trailingFilesDescription).run (),
        variablesMap);
    po::notify (variablesMap);

    if (variablesMap.count ("help"))
    {
        std::cout << "filesaver [...options] [...INPUT_FILE]" << std::endl << std::endl;
        std::cout << publicDescription << std::endl;
        return 0;
    }

    FileSaver fileSaver;

    if (variablesMap.count ("storage"))
    {
        fileSaver.setupDefaultStorage ();
    }

    if (numWorkers != 0)
    {
        fileSaver.setNumWorkers (numWorkers);
    }

    auto startTime = std::chrono::steady_clock::now ();
    fileSaver.start ();

    for (const auto& inputFile : inputFiles)
    {
        fileSaver.scan (inputFile);
    }

    while (!fileSaver.areAllTargetsFinished ())
    {
        logCurrentStatus (fileSaver, startTime);
        std::this_thread::sleep_for (std::chrono::milliseconds (300));
    }

    logCurrentStatus (fileSaver, startTime);

    for (auto& target : fileSaver.getTargets ())
    {
        std::cout << target.string () << " " << prettyPrintBytes (fileSaver.getCurrentSizeAt (target.string ()))
                  << std::endl;
    }

    fileSaver.stop ();

    return 0;
}

void CommandLineApp::logCurrentStatus (FileSaver& fileSaver,
                                       const std::chrono::steady_clock::time_point& startTime) const
{
    const long long int milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now () - startTime).count ();

    const auto totalFiles = fileSaver.getTotalFiles ();
    const double filesPerSecond =
        milliseconds > 0 ? 1000.0 * (static_cast<double> (totalFiles) / static_cast<double> (milliseconds)) : 0.0;

    const StatusDescr statusDescr{filesPerSecond,
                                  milliseconds,
                                  totalFiles,
                                  fileSaver.getStorageQueueSize (),
                                  fileSaver.getInMemoryEntryCount (),
                                  prettyPrintBytes (fileSaver.getCurrentSizeAt (fileSaver.getTargets ()[0].string ()))};
    this->statusPrinter.logStatus (statusDescr);

    std::this_thread::sleep_for (std::chrono::milliseconds (300));
}

} // namespace filesaver
