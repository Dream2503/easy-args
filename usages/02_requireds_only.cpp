// Usage: ./a.out source.txt destination.txt

#define REQUIRED_ARGS                                                                                                                                \
    REQUIRED_ARG(std::string, source, "source", "Source file")                                                                                       \
    REQUIRED_ARG(std::string, destination, "dest", "Destination file")

#include "../includes/easyargs.hpp"

int main(const int argc, char* argv[]) {
    args_t args;

    if (!args.parse_args(argc, argv)) {
        args.display();
        return 1;
    }
    std::cout << "Copying " << args.source << " to " << args.destination << std::endl;
    return 0;
}
