//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#include "CommandLineApp.h"
#include "CommandLineOptions.h"

namespace filesaver::command_line
{

namespace po = boost::program_options;

int CommandLineApp::main (int argc, char** argv)
{
    CommandLineOptions commandLineOptions = CommandLineOptions::fromArgs (argc, argv);

    if (commandLineOptions.getCommandType () == CommandType::HELP_COMMAND)
    {
        std::cout << "filesaver [...options] [...INPUT_FILE]" << std::endl << std::endl;
        std::cout << commandLineOptions.getPublicDescription () << std::endl;
        return 0;
    }

    FileSaverFactory fileSaverFactory;
    if (commandLineOptions.getCommandType () == CommandType::LIST_CATEGORIES)
    {
        return handleListCategories (fileSaverFactory);
    }

    if (commandLineOptions.getCommandType () == CommandType::CATEGORY_FIND)
    {
        auto categoryFindOptions = std::get<CategoryFindOptions> (commandLineOptions.getDetailOptions ());
        return handleCategoryFind (fileSaverFactory, categoryFindOptions);
    }

    auto scanCommandOptions = std::get<ScanCommandOptions> (commandLineOptions.getDetailOptions ());
    return handleScan (fileSaverFactory, scanCommandOptions);
}

int CommandLineApp::handleScan (FileSaverFactory& fileSaverFactory, ScanCommandOptions scanCommandOptions) const
{
    auto& fileSaver = fileSaverFactory.getRef ();

    if (scanCommandOptions.numWorkers != 0)
    {
        fileSaver.setNumWorkers (scanCommandOptions.numWorkers);
    }

    auto startTime = std::chrono::steady_clock::now ();
    fileSaver.start ();

    for (const auto& inputFile : scanCommandOptions.inputFiles)
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

    if (scanCommandOptions.server)
    {
        fileSaver.join ();
    }
    fileSaver.stop ();

    return 0;
}

int CommandLineApp::handleCategoryFind (FileSaverFactory& fileSaverFactory,
                                        CategoryFindOptions categoryFindOptions) const
{
    services::LevelDbFileCategoryStore* store =
        fileSaverFactory.getInjector ().get<services::LevelDbFileCategoryStore*> ();
    off_t limit = 10;
    off_t offset = 0;
    while (true)
    {
        auto paths = store->getPaths (categoryFindOptions.categoryName, limit, offset);
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

int CommandLineApp::handleListCategories (FileSaverFactory& fileSaverFactory) const
{
    services::FileCategoryService* fileCategoryService =
        fileSaverFactory.getInjector ().get<services::FileCategoryService*> ();
    auto categories = fileCategoryService->getCategories ();
    for (const auto& category : categories)
    {
        fmt::print ("{}: {}\n{}\n---\n", category->getTag (), category->getName (), category->getDescription ());
    }
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
