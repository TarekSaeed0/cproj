cmake_minimum_required(VERSION 3.25.0)
project(
	cproj
	VERSION 1.0
	DESCRIPTION "Simple tool to create new c project"
	LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} source/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE header)
target_link_directories(${PROJECT_NAME} PRIVATE library)
target_compile_options(
	${PROJECT_NAME}
	PRIVATE -O2
			-Werror
			-Wall
			-Wextra
			-pedantic
			-Wfloat-equal
			-Wundef
			-Wshadow
			-Wpointer-arith
			-Wcast-align
			-Wstrict-prototypes
			-Wstrict-overflow=5
			-Wwrite-strings
			-Waggregate-return
			-Wcast-qual
			-Wswitch-default
			-Wconversion
			-Wunreachable-code
)

install(TARGETS ${PROJECT_NAME})
