//
// Created by Pedro Tacla Yamada on 29/4/21.
//

#ifndef FILESAVER_COMMANDLINEOPTIONS_H
#define FILESAVER_COMMANDLINEOPTIONS_H

#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>
#include <variant>

namespace filesaver::command_line
{

namespace po = boost::program_options;

enum class LogLevel
{
    TRACE,
    DEBUG,
    INFO,
};

enum class CommandType
{
    HELP_COMMAND,
    SCAN_COMMAND,
    LIST_CATEGORIES,
    CATEGORY_FIND,
};

struct EmptyOptions {};

struct ScanCommandOptions
{
    std::vector<std::string> inputFiles{};
    unsigned int numWorkers = 0;
    bool server = false;
};

struct CategoryFindOptions
{
    std::string categoryName;
};

using DetailOptions = std::variant<EmptyOptions, ScanCommandOptions, CategoryFindOptions>;

class CommandLineOptions
{
public:
    static CommandLineOptions fromArgs (int argc, char** argv);
    LogLevel getLogLevel () const;
    CommandType getCommandType () const;
    const DetailOptions& getDetailOptions () const;
    const po::options_description& getPublicDescription () const;

private:
    CommandLineOptions () = default;

    po::options_description m_publicDescription{"General options"};

    // General options
    LogLevel m_logLevel = LogLevel::INFO;

    // Command types and command specific options
    CommandType m_commandType = CommandType::HELP_COMMAND;
    DetailOptions m_detailOptions = EmptyOptions{};
};

} // namespace filesaver::command_line

#endif // FILESAVER_COMMANDLINEOPTIONS_H
