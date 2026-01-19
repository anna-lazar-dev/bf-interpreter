#pragma once
#include <cstdint>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

class BrainfuckInterpreter {
public:
    void run(const std::string& code, std::istream& in, std::ostream& out);

    void setTapeSize(std::size_t size) { tapeSize_ = size; }
    void setMaxSteps(std::uint64_t maxSteps) { maxSteps_ = maxSteps; }
    void setDebug(bool debug) { debug_ = debug; }

    void enableStats(bool enable) { statsEnabled_ = enable; }


private:
    std::size_t tapeSize_ = 30000;
    std::uint64_t maxSteps_ = 10'000'000;
    bool debug_ = false;
    bool statsEnabled_ = false;

};
