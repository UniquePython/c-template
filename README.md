# Template

## Build

Building is driven by `make`, which wraps CMake:

```sh
make debug     # configure + build a debug binary (sanitizers, -Og, -g)
make release   # configure + build an optimized release binary (-O3)
make clean     # remove build/ and bin/
```

Built binaries are placed in `bin/`.

## Requirements

- CMake 3.20+
- A C compiler with C17 support (GCC or Clang recommended)

## Project structure

```
include/   Public headers
src/       Source files
```
