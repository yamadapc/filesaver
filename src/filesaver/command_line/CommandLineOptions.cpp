//
// Created by Pedro Tacla Yamada on 29/4/21.
//

#include "CommandLineOptions.h"

namespace filesaver::command_line
{

CommandLineOptions CommandLineOptions::fromArgs (int argc, char** argv)
{
    CommandLineOptions commandLineOptions;

    po::variables_map variablesMap;
    po::positional_options_description trailingFilesDescription;
    std::vector<std::string> inputFiles{};
    unsigned int numWorkers = 0;
    std::string lsCategoryName = "";

    // clang-format off
    commandLineOptions.m_publicDescription.add_options ()
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

    po::store (po::command_line_parser (argc, argv)
                   .options (commandLineOptions.m_publicDescription)
                   .positional (trailingFilesDescription)
                   .run (),
               variablesMap);
    po::notify (variablesMap);

    if (variablesMap.count ("debug"))
    {
        commandLineOptions.m_logLevel = LogLevel::DEBUG;
        spdlog::set_level (spdlog::level::debug);
    }
    else if (variablesMap.count ("trace"))
    {
        commandLineOptions.m_logLevel = LogLevel::TRACE;
        spdlog::set_level (spdlog::level::trace);
    }

    if (variablesMap.count ("help"))
    {
        commandLineOptions.m_commandType = CommandType::HELP_COMMAND;
        return commandLineOptions;
    }

    if (variablesMap.count ("categories"))
    {
        commandLineOptions.m_commandType = CommandType::LIST_CATEGORIES;
        return commandLineOptions;
    }

    if (variablesMap.count ("category-find"))
    {
        commandLineOptions.m_commandType = CommandType::CATEGORY_FIND;
        CategoryFindOptions categoryFindOptions{};
        categoryFindOptions.categoryName = lsCategoryName;
        commandLineOptions.m_detailOptions = categoryFindOptions;
        return commandLineOptions;
    }

    commandLineOptions.m_commandType = CommandType::SCAN_COMMAND;
    ScanCommandOptions scanCommandOptions{};
    scanCommandOptions.inputFiles = inputFiles;
    scanCommandOptions.numWorkers = numWorkers;
    scanCommandOptions.server = variablesMap.count ("server");
    commandLineOptions.m_detailOptions = scanCommandOptions;
    return commandLineOptions;
}

LogLevel CommandLineOptions::getLogLevel () const
{
    return m_logLevel;
}

CommandType CommandLineOptions::getCommandType () const
{
    return m_commandType;
}

const DetailOptions& CommandLineOptions::getDetailOptions () const
{
    return m_detailOptions;
}

const po::options_description& CommandLineOptions::getPublicDescription () const
{
    return m_publicDescription;
}

} // namespace filesaver::command_line
