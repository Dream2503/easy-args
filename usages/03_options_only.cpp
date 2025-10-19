// Usage: ./config [-c <config>] [-v] [--disable-cache] [-h]

// No required arguments for this example

#define OPTIONAL_ARGS OPTIONAL_ARG(std::string, config_file, "config.ini", "-c", "config", "Configuration file path")

#define BOOLEAN_ARGS                                                                                                                                 \
    BOOLEAN_ARG(verbose, "-v", "Verbose output")                                                                                                     \
    BOOLEAN_ARG(no_cache, "--disable-cache", "Disable the use of cache")                                                                             \
    BOOLEAN_ARG(help, "-h", "Show help")

#include "../includes/easyargs.hpp"

int main(const int argc, char* argv[]) {
    args_t args;
    args.parse_args(argc, argv);

    if (args.help) {
        args.display();
        return 1;
    }
    std::cout << "Configuration file: " << args.config_file << std::endl;
    std::cout << "Verbose: " << args.verbose << std::endl;
    std::cout << "Using cache: " << args.no_cache << std::endl;
    return 0;
}
