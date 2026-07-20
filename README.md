# Template

## Build

Building is driven by `make`, which wraps CMake:

```sh
make debug     # configure + build a debug binary (sanitizers, -Og, -g, TEMPLATE_VERBOSITY=6)
make dev       # configure + build a dev binary (sanitizers, -Og, -g, TEMPLATE_VERBOSITY=5)
make release   # configure + build an optimized release binary (-O3, TEMPLATE_VERBOSITY=3)
make clean     # remove build/ and bin/
```

Built binaries are placed in `bin/`.

`TEMPLATE_VERBOSITY` controls which log levels are compiled in — see
`include/template_log.h` for the full tier breakdown (`FATAL` up through
`TRACE`). `debug` and `dev` both build with sanitizers and `-fanalyzer`;
`dev` is the same tooling as `debug` but one tier quieter (no `TRACE`),
useful when you want sanitizers without `TRACE`-level log spam.

### Checking for stubs

```sh
make check-release-ready
```

Greps `src/` and `include/` (excluding `template_log.h` and `template_assert.h` itself) for any
remaining `LOG_NOT_IMPLEMENTED(...)` and/or `ASSERT_DEBUG(...)` calls and fails if any are found. Not
wired into CI or into any build target automatically — run it manually
before you consider something done.

## Requirements

- CMake 3.20+
- A C compiler with C17 support (GCC or Clang required)

## Project structure

```
include/   Public headers
src/       Source files
```