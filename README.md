# Custom C++ Shell

A POSIX-compliant command-line shell written in C++ for the CodeCrafters "Build Your Own Shell" challenge. 

## Features
- **Built-in Commands:** 
  - `cd` (supports relative/absolute paths and `~` expansion to `$HOME`)
  - `echo`
  - `pwd`
  - `type` (identifies built-ins vs external executables)
  - `exit`
- **Path Resolution:** Parses the `$PATH` environment variable to locate and execute external binaries using `access()`.
- **I/O Redirection:** 
  - Standard output redirection (`>`, `1>`, `>>`, `1>>`)
  - Standard error redirection (`2>`, `2>>`)
  - Safely handles file descriptors using `dup()`, `dup2()`, and `fcntl.h` flags.

## Tech Stack & System Calls
- **Language:** C++ (C++17 required for `<filesystem>`)
- **Key Headers:** `<unistd.h>`, `<fcntl.h>`, `<filesystem>`
- **System Calls Used:** `chdir()`, `access()`, `dup()`, `dup2()`, `open()`, `close()`, `std::system()`

## Build and Run

1. Compile the code:
   ```bash
   g++ -std=c++17 main.cpp -o shell