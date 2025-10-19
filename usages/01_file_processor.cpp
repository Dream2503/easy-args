// Usage: ./a.out <input> <output> [-t <threads>] [-h]

// 1. Set up your arguments
#define REQUIRED_ARGS                                                                                                                                \
    REQUIRED_ARG(std::string, input_file, "input", "Input file path")                                                                                \
    REQUIRED_ARG(std::string, output_file, "output", "Output file path")

#define OPTIONAL_ARGS OPTIONAL_ARG(uint32_t, threads, 1, "-t", "threads", "Number of threads to use")

#define BOOLEAN_ARGS BOOLEAN_ARG(help, "-h", "Show help")

#include "../includes/easyargs.hpp"

int main(const int argc, char* argv[]) {
    args_t args; // 2. Start with default arguments

    // 3. If parsing fails OR help argument is passed, print help
    if (!args.parse_args(argc, argv) || args.help) {
        args.display();
        return 1;
    }
    // 4. Use arguments
    std::cout << "Processing " << args.input_file << " -> " << args.output_file << std::endl;
    std::cout << "Threads: " << args.threads << std::endl;
    return 0;
}
