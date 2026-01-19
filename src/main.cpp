#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdint>

static std::string readAllText(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static void printUsage() {
    std::cout
        << "Usage:\n"
        << "  bf run <program.bf> [--input <input.txt>] [--debug] [--stats] [--max-steps N]\n\n"
        << "Examples:\n"
        << "  bf run examples/echo.bf\n"
        << "  bf run examples/echo.bf --stats\n"
        << "  bf run program.bf --debug --max-steps 5000000\n";
}

int main(int argc, char** argv) {
    try {
        if (argc < 3) {
            printUsage();
            return 1;
        }

        std::string command = argv[1];
        if (command != "run") {
            printUsage();
            return 1;
        }

        std::string programPath = argv[2];
        std::string inputPath;

        bool debug = false;
        bool stats = false;
        std::uint64_t maxSteps = 10'000'000;

        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "--input" && i + 1 < argc) {
                inputPath = argv[++i];
            } else if (arg == "--debug") {
                debug = true;
            } else if (arg == "--stats") {
                stats = true;
            } else if (arg == "--max-steps" && i + 1 < argc) {
                maxSteps = std::stoull(argv[++i]);
            } else {
                throw std::runtime_error("Unknown argument: " + arg);
            }
        }

        std::string code = readAllText(programPath);

        BrainfuckInterpreter bf;
        bf.setDebug(debug);
        bf.setMaxSteps(maxSteps);
        bf.enableStats(stats);

        std::ifstream inputFile;
        if (!inputPath.empty()) {
            inputFile.open(inputPath, std::ios::binary);
            if (!inputFile) {
                throw std::runtime_error("Cannot open input file: " + inputPath);
            }
        }

        std::istream& in = inputPath.empty() ? std::cin : inputFile;
        bf.run(code, in, std::cout);

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }
}
