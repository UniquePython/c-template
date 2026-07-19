cmake_minimum_required(VERSION 3.20)

project(template LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.c
)

add_executable(template ${SOURCES})

set_target_properties(template PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

target_include_directories(template PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_options(template PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wcast-qual
    -Wwrite-strings
    -Wformat=2
    -Wundef
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wold-style-definition
    -Wimplicit-fallthrough
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wcast-align
    -Wvla
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat-overflow=2
    -Wformat-truncation=2
    -Walloc-zero
    -Warray-bounds=2
    -Wstringop-overflow=4
)

target_compile_options(template PRIVATE
    $<$<CONFIG:Debug>:
        -Og
        -g
        -fanalyzer
        -fstack-protector-strong
        -fno-omit-frame-pointer
        -fsanitize=address,undefined,leak
    >
)

target_link_options(template PRIVATE
    $<$<CONFIG:Debug>:
        -fsanitize=address,undefined,leak
    >
)

target_compile_definitions(template PRIVATE
    $<$<CONFIG:Release>:VERBOSITY=6>
)

target_compile_options(template PRIVATE
    $<$<CONFIG:Dev>:
        -Og
        -g
        -fanalyzer
        -fstack-protector-strong
        -fno-omit-frame-pointer
        -fsanitize=address,undefined,leak
    >
)

target_link_options(template PRIVATE
    $<$<CONFIG:Dev>:
        -fsanitize=address,undefined,leak
    >
)

target_compile_definitions(template PRIVATE
    $<$<CONFIG:Release>:VERBOSITY=5>
)

target_compile_options(template PRIVATE
    $<$<CONFIG:Release>:-O3>
)

target_compile_definitions(template PRIVATE
    $<$<CONFIG:Release>:VERBOSITY=3>
)
