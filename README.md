# EasyArgs

A simple, single-header argument parser for C that eliminates boilerplate and code duplication.

## Features

- **Single header file:** Just include `easyargs.h` and you're good to go
- **Zero duplication:** Define your arguments once to get struct fields, parsing, and help generation automatically
- **Type-safe:** Supports all common C types with built-in parsing
- **Flexible:** Handle required arguments, optional arguments with defaults, and boolean flags
- **Automatic help generation:** Professional-looking usage and help text generated from your definitions

## Quick Start

Check out ./examples to see some use cases!

### 1. Define your arguments

```c++
// Define required positional arguments
#define REQUIRED_ARGS                                                                                   \
    REQUIRED_ARG(std::string, input_file, "input", "Input file path")                                   \
    REQUIRED_ARG(std::string, output_file, "output", "Output file path")

// Define optional arguments with defaults
#define OPTIONAL_ARGS OPTIONAL_ARG(uint32_t, threads, 1, "-t", "threads", "Number of threads to use")

// Define boolean flags
#define BOOLEAN_ARGS BOOLEAN_ARG(help, "-h", "Show help")
```

### 2. Include EasyArgs

```c++
#include "easyargs.hpp"
```

### 3. Use it in your main function

```c++
int main(const int argc, char* argv[]) {
    args_t args; // Initialize with the default values specified above

    // Parse arguments
    if (!args.parse_args(argc, argv) || args.help) {
        args.display();
        return 1;
    }
    // Use your arguments
    std::cout << "Processing " << args.input_file << " -> " << args.output_file << std::endl;
    std::cout << "Threads: " << args.threads << std::endl;
    return 0;
}
```

### 4. Compile and Run

```bash
g++ file_processor.c -o file_processor
./file_processor input.txt output.txt -t 4
```

## Argument Types

### Required Arguments

Required arguments are positional and must be provided in order:

```c++
#define REQUIRED_ARGS                                                   \
    REQUIRED_ARG(std::string, name, "label", "description")             \
    REQUIRED_ARG(size_t, count, "count", "Number of items")             \
    REQUIRED_ARG(double, threshold, "threshold", "Processing threshold")
```

**Supported types:** Any type that implements `operator>>` for `std::ostream` can be used as an argument type.

### Optional Arguments

Optional arguments have flags and default values:

```c++
#define OPTIONAL_ARGS                                                                           \
    OPTIONAL_ARG(int, verbose, 0, "-v", "level", "Verbosity level")                             \
    OPTIONAL_ARG(std::string, config, "config.ini", "--config", "file", "Configuration file")   \
    OPTIONAL_ARG(double, factor, 1.5, "-f", "factor", "Scaling factor", 2)
```

**Supported types:** Same as required arguments

### Boolean Arguments

Boolean flags toggle between true and false if present or missing, respectively:

```c++
#define BOOLEAN_ARGS                                                \
    BOOLEAN_ARG(verbose, "-v", "Enable verbose output")             \
    BOOLEAN_ARG(quiet, "-q", "Suppress all output")                 \
    BOOLEAN_ARG(force, "--force", "Force overwrite existing files")
```

## Advanced Usage

### Custom Parsing

For basic types, parsing is handled automatically. Any type that implements `operator>>` can be parsed without additional code.

```c++
#define REQUIRED_ARG(type, name, label, description)
```

## Installation

1. Download `easyargs.hpp`
2. Place it in your project directory or include path
3. Include it in your source files: `#include "easyargs.hpp"`

No compilation or linking required &mdash; it's header-only!
