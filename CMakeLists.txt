cmake_minimum_required(VERSION 3.25.0)
project(cproj)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} source/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE header)
target_compile_options(${PROJECT_NAME} PRIVATE
    -O2 -Werror -Wall -Wextra -pedantic -Wfloat-equal -Wundef
    -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5
    -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum
    -Wconversion -Wunreachable-code
)
