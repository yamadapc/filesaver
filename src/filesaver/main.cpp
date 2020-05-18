#include "command_line/CommandLineApp.h"

int main (int argc, char* argv[])
{
    filesaver::CommandLineApp commandLineApp{};
    return commandLineApp.main (argc, argv);
}