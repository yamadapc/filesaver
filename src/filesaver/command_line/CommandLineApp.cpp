//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <lfilesaver/factory/FileSaverFactory.h>
#include <spdlog/spdlog.h>

#include "CommandLineApp.h"

namespace filesaver::command_line
{

namespace po = boost::program_options;

int CommandLineApp::main (int argc, char** argv)
{
    po::options_description publicDescription ("General options");
    po::variables_map variablesMap;
    po::positional_options_description trailingFilesDescription;
    std::vector<std::string> inputFiles{};
    unsigned int numWorkers = 0;
    std::string lsCategoryName = "";

    // clang-format off
    publicDescription.add_options ()
        ("help,h", "print this help message")
        ("debug", "Enable debug logging")
        ("trace", "Enable trace logging")
        ("server", "Stay running as HTTP server")
        ("num-workers", po::value (&numWorkers), "The number of worker threads to use")
        ("categories", "List existing file categories")
        ("category-find", po::value (&lsCategoryName), "List entries of a certain category")
        ("input-file", po::value (&inputFiles), "input file");
    // clang-format on

    trailingFilesDescription.add ("input-file", -1);

    po::store (
        po::command_line_parser (argc, argv).options (publicDescription).positional (trailingFilesDescription).run (),
        variablesMap);
    po::notify (variablesMap);

    if (variablesMap.count ("debug"))
    {
        spdlog::set_level (spdlog::level::debug);
    }

    if (variablesMap.count ("trace"))
    {
        spdlog::set_level (spdlog::level::trace);
    }

    if (variablesMap.count ("help"))
    {
        std::cout << "filesaver [...options] [...INPUT_FILE]" << std::endl << std::endl;
        std::cout << publicDescription << std::endl;
        return 0;
    }

    if (variablesMap.count ("categories"))
    {
        services::FileCategoryServiceImpl fileCategoryService;
        auto categories = fileCategoryService.getCategories ();
        for (const auto& category : categories)
        {
            fmt::print ("{}: {}\n{}\n---\n", category->getTag (), category->getName (), category->getDescription ());
        }
        return 0;
    }

    if (variablesMap.count ("category-find"))
    {
        services::settings::SettingsService settingsService = services::settings::SettingsService::defaultForMac ();
        services::LevelDbFactory factory{&settingsService};
        services::LevelDbFileCategoryStore store{&factory};
        off_t limit = 10;
        off_t offset = 0;
        while (true)
        {
            auto paths = store.getPaths (lsCategoryName, limit, offset);
            if (paths.size () == 0)
            {
                break;
            }
            for (const auto& path : paths)
            {
                fmt::print ("{}\n", path);
            }
            offset += limit;
        }
        return 0;
    }

    FileSaverFactory fileSaverFactory;
    auto& fileSaver = fileSaverFactory.getRef ();

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
        std::this_thread::sleep_for (std::chrono::milliseconds (CLI_LOG_DELAY));
    }

    logCurrentStatus (fileSaver, startTime);

    for (auto& target : fileSaver.getTargets ())
    {
        std::cout << target.string () << " " << utils::prettyPrintBytes (fileSaver.getCurrentSizeAt (target.string ()))
                  << std::endl;
    }

    if (variablesMap.count ("server"))
    {
        fileSaver.join ();
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

    const auto& targets = fileSaver.getTargets ();
    for (size_t i = 0; i < targets.size (); i++)
    {
        const logger::StatusDescr statusDescr{
            filesPerSecond,
            milliseconds,
            totalFiles,
            utils::prettyPrintBytes (fileSaver.getCurrentSizeAt (fileSaver.getTargets ()[0].string ()))};
        this->statusPrinter.logStatus (statusDescr);
    }

    std::this_thread::sleep_for (std::chrono::milliseconds (CLI_LOG_DELAY));
}

} // namespace filesaver::command_line
