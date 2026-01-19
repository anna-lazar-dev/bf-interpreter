#include "interpreter.h"

#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <stack>
#include <iostream>

static std::unordered_map<std::size_t, std::size_t>
buildBracketMap(const std::string& code) {
    std::unordered_map<std::size_t, std::size_t> map;
    std::stack<std::size_t> stack;

    for (std::size_t i = 0; i < code.size(); ++i) {
        if (code[i] == '[') {
            stack.push(i);
        } else if (code[i] == ']') {
            if (stack.empty()) {
                throw std::runtime_error(
                    "Unmatched ']' at position " + std::to_string(i)
                );
            }
            std::size_t open = stack.top();
            stack.pop();
            map[open] = i;
            map[i] = open;
        }
    }

    if (!stack.empty()) {
        throw std::runtime_error(
            "Unmatched '[' at position " + std::to_string(stack.top())
        );
    }

    return map;
}

void BrainfuckInterpreter::run(
    const std::string& code,
    std::istream& in,
    std::ostream& out
) {
    auto bracketMap = buildBracketMap(code);

    std::vector<std::uint8_t> tape(tapeSize_, 0);
    std::size_t ptr = 0;
    std::size_t ip = 0;
    std::uint64_t steps = 0;

    std::unordered_map<char, std::uint64_t> stats;

    auto bump = [&](char c) {
        if (statsEnabled_) {
            ++stats[c];
        }
    };

    auto debugPrint = [&](char op) {
        if (!debug_) return;
        out << "[DBG] ip=" << ip
            << " op=" << op
            << " ptr=" << ptr
            << " val=" << static_cast<int>(tape[ptr])
            << "\n";
    };

    while (ip < code.size()) {
        if (++steps > maxSteps_) {
            throw std::runtime_error(
                "Max steps exceeded (possible infinite loop)"
            );
        }

        char op = code[ip];

        switch (op) {
            case '>':
                bump('>');
                debugPrint(op);
                if (ptr + 1 >= tape.size()) {
                    throw std::runtime_error("Pointer moved out of bounds (>)");
                }
                ++ptr;
                break;

            case '<':
                bump('<');
                debugPrint(op);
                if (ptr == 0) {
                    throw std::runtime_error("Pointer moved out of bounds (<)");
                }
                --ptr;
                break;

            case '+':
                bump('+');
                debugPrint(op);
                ++tape[ptr];
                break;

            case '-':
                bump('-');
                debugPrint(op);
                --tape[ptr];
                break;

            case '.':
                bump('.');
                debugPrint(op);
                out << static_cast<char>(tape[ptr]);
                break;

            case ',':
                bump(',');
                debugPrint(op);
                {
                    int c = in.get();
                    tape[ptr] = (c == EOF)
                        ? 0
                        : static_cast<std::uint8_t>(c);
                }
                break;

            case '[':
                bump('[');
                debugPrint(op);
                if (tape[ptr] == 0) {
                    ip = bracketMap.at(ip);
                }
                break;

            case ']':
                bump(']');
                debugPrint(op);
                if (tape[ptr] != 0) {
                    ip = bracketMap.at(ip);
                }
                break;

            default:
                // ignore non-command characters
                break;
        }

        ++ip;
    }

    if (statsEnabled_) {
        out << "\n--- Execution statistics ---\n";
        const char commands[] = {'>', '<', '+', '-', '.', ',', '[', ']'};
        for (char c : commands) {
            out << c << " : " << stats[c] << "\n";
        }
    }
}

