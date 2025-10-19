#pragma once

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

// #define REQUIRED_ARG(type, name, label, description)
// #define OPTIONAL_ARG(type, name, _default, flag, label, description)
// #define BOOLEAN_ARG(name, flag, description)

// count arguments
#ifdef REQUIRED_ARGS
#define REQUIRED_ARG(...) +1
constexpr uint32_t REQUIRED_ARGS_COUNT = REQUIRED_ARGS;
#undef REQUIRED_ARG
#else
constexpr uint32_t REQUIRED_ARGS_COUNT = 0;
#endif

#ifdef OPTIONAL_ARGS
#define OPTIONAL_ARG(...) +1
constexpr uint32_t OPTIONAL_ARGS_COUNT = 0 OPTIONAL_ARGS;
#undef OPTIONAL_ARG
#else
constexpr uint32_t OPTIONAL_ARGS_COUNT = 0;
#endif

#ifdef BOOLEAN_ARGS
#define BOOLEAN_ARG(...) +1
constexpr uint32_t BOOLEAN_ARGS_COUNT = 0 BOOLEAN_ARGS;
#undef BOOLEAN_ARG
#else
constexpr uint32_t BOOLEAN_ARGS_COUNT = 0;
#endif

struct args_t {
    std::string file_name;

#ifdef REQUIRED_ARGS
#define REQUIRED_ARG(type, name, ...) type name;
    REQUIRED_ARGS
#undef REQUIRED_ARG
#endif

#ifdef OPTIONAL_ARGS
#define OPTIONAL_ARG(type, name, ...) type name;
    OPTIONAL_ARGS
#undef OPTIONAL_ARG
#endif

#ifdef BOOLEAN_ARGS
#define BOOLEAN_ARG(name, ...) bool name;
    BOOLEAN_ARGS
#undef BOOLEAN_ARG
#endif

    template <typename T>
    static T parse(const char*);

    args_t();
    bool parse_args(int argc, char** argv);
    void display();
};

// Input parser
template <typename T>
T args_t::parse(const char* string) {
    T value;

    if (!(std::stringstream(string) >> value)) {
        std::cerr << "Invalid conversion: " << string;
        std::exit(1);
    }
    return value;
}

// Build an args_t struct with assigned default values
inline args_t::args_t() {
    file_name = std::string();

#ifdef REQUIRED_ARGS
#define REQUIRED_ARG(type, name, ...) name = type();
    REQUIRED_ARGS
#undef REQUIRED_ARG
#endif

#ifdef OPTIONAL_ARGS
#define OPTIONAL_ARG(type, name, _default, ...) name = _default;
    OPTIONAL_ARGS
#undef OPTIONAL_ARG
#endif

#ifdef BOOLEAN_ARGS
#define BOOLEAN_ARG(name, ...) name = false;
    BOOLEAN_ARGS
#undef BOOLEAN_ARG
#endif
}

// Parse arguments. Returns false if failed.
inline bool args_t::parse_args(const int argc, char** argv) {
    if (!argc || !argv) {
        std::cerr << "Internal error: null args or argv." << std::endl;
        return false;
    }
    int i = 1;
    file_name = argv[0];

    // If not enough required arguments
    if (argc < 1 + REQUIRED_ARGS_COUNT) {
        std::cerr << "Not all required arguments included." << std::endl;
        return false;
    }

// Get required arguments
#ifdef REQUIRED_ARGS
#define REQUIRED_ARG(type, name, label, description) name = parse<type>(argv[i++]);
    REQUIRED_ARGS
#undef REQUIRED_ARG
#endif

// Get optional and boolean arguments
#define OPTIONAL_ARG(type, name, _default, flag, label, description)                                                                                 \
    if (std::string(argv[i]) == flag) {                                                                                                              \
        if (i + 1 >= argc) {                                                                                                                         \
            std::cerr << "Error: option '" << flag << "' requires a value." << std::endl;                                                            \
            return 0;                                                                                                                                \
        }                                                                                                                                            \
        name = parse<type>(argv[++i]);                                                                                                               \
        i++;                                                                                                                                         \
        continue;                                                                                                                                    \
    }

#define BOOLEAN_ARG(name, flag, description)                                                                                                         \
    if (std::string(argv[i]) == flag) {                                                                                                              \
        name = true;                                                                                                                                 \
        i++;                                                                                                                                         \
        continue;                                                                                                                                    \
    }

    while (i < argc) {
#ifdef OPTIONAL_ARGS
        OPTIONAL_ARGS
#endif

#ifdef BOOLEAN_ARGS
        BOOLEAN_ARGS
#endif
        std::cerr << "Warning: Ignoring invalid argument '" << argv[i] << '\'' << std::endl;
    }
#undef OPTIONAL_ARG
#undef BOOLEAN_ARG
    return true;
}

// Display help string, given command used to launch program, e.g., argv[0]
inline void args_t::display() {
    // USAGE SECTION
    std::cout << "USAGE:" << std::endl;
    std::cout << "    " << file_name << ' ';

#ifdef REQUIRED_ARGS
    if constexpr (REQUIRED_ARGS_COUNT <= 3) {
#define REQUIRED_ARG(type, name, label, ...) "<" label "> "
        std::cout << REQUIRED_ARGS;
#undef REQUIRED_ARG
    } else {
        std::cout << "<ARGUMENTS> ";
    }
#endif

    if constexpr (OPTIONAL_ARGS_COUNT + BOOLEAN_ARGS_COUNT <= 3) {
#ifdef OPTIONAL_ARGS
#define OPTIONAL_ARG(type, name, _default, flag, label, ...) "[" flag " <" label ">] "
        std::cout << OPTIONAL_ARGS;
#undef OPTIONAL_ARG
#endif

#ifdef BOOLEAN_ARGS
#define BOOLEAN_ARG(name, flag, ...) "[" flag "] "
        std::cout << BOOLEAN_ARGS;
#undef BOOLEAN_ARG
#endif
    } else {
        std::cout << "[OPTIONS]";
    }
    std::cout << std::endl << std::endl;

    // Get maximum width of labels for spacing
    uint64_t max_width = 0;
#ifdef REQUIRED_ARGS
#define REQUIRED_ARG(type, name, label, ...) max_width = std::max(max_width, std::strlen(label) + 2);
    REQUIRED_ARGS
#undef REQUIRED_ARG
#endif

#ifdef OPTIONAL_ARGS
#define OPTIONAL_ARG(type, name, _default, flag, label, ...) max_width = std::max(max_width, std::strlen(flag) + 1 + std::strlen(label) + 2);
    OPTIONAL_ARGS
#undef OPTIONAL_ARG
#endif

#ifdef BOOLEAN_ARGS
#define BOOLEAN_ARG(name, flag, ...) max_width = std::max(max_width, std::strlen(flag));
    BOOLEAN_ARGS
#undef BOOLEAN_ARG
#endif

// ARGUMENTS SECTION
#ifdef REQUIRED_ARGS
    std::cout << "ARGUMENTS:" << std::endl;

#define REQUIRED_ARG(type, name, label, description, ...)                                                                                            \
    std::cout << "    <" label ">" << std::string(max_width - std::strlen(label) - 2, ' ') << "    " description << std::endl;
    REQUIRED_ARGS
#undef REQUIRED_ARG

    std::cout << std::endl;
#endif

#if defined(OPTIONAL_ARGS) || defined(BOOLEAN_ARGS)
    std::cout << "OPTIONS:" << std::endl;

#ifdef OPTIONAL_ARGS

#define OPTIONAL_ARG(type, name, _default, flag, label, description)                                                                                 \
    std::cout << "    " flag " <" label ">" << std::string(max_width - std::strlen(label) - std::strlen(flag) - 3, ' ');                             \
    std::cout << "    " description " (default: " << _default << ")" << std::endl;
    OPTIONAL_ARGS
#undef OPTIONAL_ARG
#endif

#ifdef BOOLEAN_ARGS
#define BOOLEAN_ARG(name, flag, description)                                                                                                         \
    std::cout << "    " flag << std::string(max_width - std::strlen(flag), ' ') << "    " description << std::endl;
    BOOLEAN_ARGS
#undef BOOLEAN_ARG
#endif

#endif
}
