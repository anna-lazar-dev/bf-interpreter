# Brainf*ck Interpreter (C++)

A simple and fast Brainf*ck interpreter written in **C++17**.  
Supports file input, interactive input, debug mode, execution statistics, and protection from infinite loops.

This project is implemented as a **CLI (command-line) application** and built with **CMake**.

---

## Features

- Supports all 8 Brainf*ck commands: `><+-.,[]`
- File input and interactive console input
- Debug mode with instruction tracing (`--debug`)
- Execution statistics (`--stats`)
- Protection from infinite loops (`--max-steps`)
- Cross-platform build with CMake
- Clean project structure suitable for GitHub portfolio

---

## Demo

### Run a program
```bash
bf run program.bf

```
### Echo example (interactive)
``` bach
bf run examples/echo.bf
hello
hello
world
world

```

### Statistics mode
``` bach
bf run program.bf --stats

```

### Example output
``` bach
A

--- Execution statistics ---
> : 9
< : 8
+ : 73
- : 8
. : 1
, : 0
[ : 1
] : 8

```

# Installation

### Build from source

``` bach
git clone https://github.com/<anna-lazar-dev>/bf-interpreter.git
cd bf-interpreter
cmake -S . -B build
cmake --build build --config Release


```