cmake_minimum_required(VERSION 3.25.0)
project(
	cproj
	VERSION 1.0
	DESCRIPTION "Simple tool to create new c projects"
	LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} src/main.c)
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
