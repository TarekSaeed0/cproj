#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
	assert(argc == 2);
	const char *project_path = argv[1];
	assert(mkdir(project_path, S_IRWXU | S_IRWXG | S_IRWXO) != -1);

	const char *project_name = strrchr(project_path, '/');
	if (project_name != NULL)
		++project_name;
	else
		project_name = project_path;

	FILE *cmake_version_command_file = popen("cmake --version", "r");
	assert(cmake_version_command_file != NULL);
	char cmake_version_command_buffer[64];
	assert(
		fgets(
			cmake_version_command_buffer,
			sizeof cmake_version_command_buffer,
			cmake_version_command_file
		) != NULL
	);
	char *cmake_version = strstr(cmake_version_command_buffer, "version ");
	assert(cmake_version != NULL);
	cmake_version += strlen("version ");
	cmake_version[strcspn(cmake_version, "\n")] = '\0';
	assert(pclose(cmake_version_command_file) != -1);

	char *project_cmakelists_txt_path =
		malloc(strlen(project_path) + strlen("/CMakeLists.txt") + 1);
	assert(project_cmakelists_txt_path != NULL);
	strcat(
		strcpy(project_cmakelists_txt_path, project_path),
		"/CMakeLists.txt"
	);
	FILE *project_cmakelists_txt_file = fopen(project_cmakelists_txt_path, "w");
	assert(project_cmakelists_txt_file != NULL);
	free(project_cmakelists_txt_path);
	assert(
		fprintf(
			project_cmakelists_txt_file,
			"cmake_minimum_required(VERSION %s)\n"
			"project(%s)\n"
			"\n"
			"set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n"
			"set(CMAKE_BUILD_TYPE Debug)\n"
			"\n"
			"add_executable(${PROJECT_NAME} source/main.c)\n"
			"target_include_directories(${PROJECT_NAME} PRIVATE header)\n"
			"target_link_directories(${PROJECT_NAME} PRIVATE library)\n"
			"target_compile_options(\n"
			"	${PROJECT_NAME}\n"
			"	PRIVATE -O2\n"
			"			-Werror\n"
			"			-Wall\n"
			"			-Wextra\n"
			"			-pedantic\n"
			"			-Wfloat-equal\n"
			"			-Wundef\n"
			"			-Wshadow\n"
			"			-Wpointer-arith\n"
			"			-Wcast-align\n"
			"			-Wstrict-prototypes\n"
			"			-Wstrict-overflow=5\n"
			"			-Wwrite-strings\n"
			"			-Waggregate-return\n"
			"			-Wcast-qual\n"
			"			-Wswitch-default\n"
			"			-Wswitch-enum\n"
			"			-Wconversion\n"
			"			-Wunreachable-code\n"
			")\n",
			cmake_version,
			project_name
		) > 0
	);
	assert(fclose(project_cmakelists_txt_file) != EOF);

	char *project_cmake_format_py_path =
		malloc(strlen(project_path) + strlen("/.cmake-format.py") + 1);
	assert(project_cmake_format_py_path != NULL);
	strcat(
		strcpy(project_cmake_format_py_path, project_path),
		"/.cmake-format.py"
	);
	FILE *project_cmake_format_py_file =
		fopen(project_cmake_format_py_path, "w");
	assert(project_cmake_format_py_file != NULL);
	free(project_cmake_format_py_path);
	assert(
		fputs(
			"with section(\"format\"):\n"
			"    tab_size = 4\n"
			"    use_tabchars = True\n"
			"    dangle_parens = True\n",
			project_cmake_format_py_file
		) > 0
	);
	assert(fclose(project_cmake_format_py_file) != EOF);

	char *project_clang_format_path =
		malloc(strlen(project_path) + strlen("/.clang-format") + 1);
	assert(project_clang_format_path != NULL);
	strcat(strcpy(project_clang_format_path, project_path), "/.clang-format");
	FILE *project_clang_format_file = fopen(project_clang_format_path, "w");
	assert(project_clang_format_file != NULL);
	free(project_clang_format_path);
	assert(
		fputs(
			"UseTab: Always\n"
			"IndentWidth: 4\n"
			"TabWidth: 4\n"
			"\n"
			"AlignAfterOpenBracket: BlockIndent\n"
			"\n"
			"IndentCaseLabels: true\n"
			"  \n"
			"Cpp11BracedListStyle: false\n"
			"\n"
			"AllowAllArgumentsOnNextLine: false\n"
			"AllowAllParametersOfDeclarationOnNextLine: false\n"
			"BinPackArguments: false\n"
			"BinPackParameters: false\n",
			project_clang_format_file
		) > 0
	);
	assert(fclose(project_clang_format_file) != EOF);

	char *project_compile_commands_json_path =
		malloc(strlen(project_path) + strlen("/compile_commands.json") + 1);
	assert(project_compile_commands_json_path != NULL);
	strcat(
		strcpy(project_compile_commands_json_path, project_path),
		"/compile_commands.json"
	);
	assert(
		symlink(
			"build/compile_commands.json",
			project_compile_commands_json_path
		) != -1
	);
	free(project_compile_commands_json_path);

	char *project_source_path =
		malloc(strlen(project_path) + strlen("/source") + 1);
	assert(project_source_path != NULL);
	strcat(strcpy(project_source_path, project_path), "/source");
	assert(mkdir(project_source_path, S_IRWXU | S_IRWXG | S_IRWXO) != -1);

	char *project_source_main_c_path =
		malloc(strlen(project_source_path) + strlen("/main.c") + 1);
	assert(project_source_main_c_path != NULL);
	strcat(strcpy(project_source_main_c_path, project_source_path), "/main.c");
	free(project_source_path);
	FILE *project_source_main_c_file = fopen(project_source_main_c_path, "w");
	assert(project_source_main_c_file != NULL);
	free(project_source_main_c_path);
	assert(
		fprintf(
			project_source_main_c_file,
			"#include <stdio.h>\n"
			"\n"
			"int main(void) {\n"
			"	printf(\"Hello, World!\\n\");\n"
			"}\n"
		) > 0
	);
	assert(fclose(project_source_main_c_file) != EOF);

	char *project_resource_path =
		malloc(strlen(project_path) + strlen("/resource") + 1);
	assert(project_resource_path != NULL);
	strcat(strcpy(project_resource_path, project_path), "/resource");
	assert(mkdir(project_resource_path, S_IRWXU | S_IRWXG | S_IRWXO) != -1);
	free(project_resource_path);

	char *project_header_path =
		malloc(strlen(project_path) + strlen("/header") + 1);
	assert(project_header_path != NULL);
	strcat(strcpy(project_header_path, project_path), "/header");
	assert(mkdir(project_header_path, S_IRWXU | S_IRWXG | S_IRWXO) != -1);
	free(project_header_path);

	char *project_library_path =
		malloc(strlen(project_path) + strlen("/library") + 1);
	assert(project_library_path != NULL);
	strcat(strcpy(project_library_path, project_path), "/library");
	assert(mkdir(project_library_path, S_IRWXU | S_IRWXG | S_IRWXO) != -1);
	free(project_library_path);

	char *project_build_path =
		malloc(strlen(project_path) + strlen("/build") + 1);
	assert(project_build_path != NULL);
	strcat(strcpy(project_build_path, project_path), "/build");

	char *cmake_build_command = malloc(
		strlen("cmake -S ") + strlen(project_path) + strlen(" -B ") +
		strlen(project_build_path) + strlen(" > /dev/null") + 1
	);
	assert(cmake_build_command != NULL);
	strcat(
		strcat(
			strcat(
				strcat(strcpy(cmake_build_command, "cmake -S "), project_path),
				" -B "
			),
			project_build_path
		),
		" > /dev/null"
	);
	free(project_build_path);
	assert(system(cmake_build_command) == 0);
	free(cmake_build_command);

	return EXIT_SUCCESS;
}
