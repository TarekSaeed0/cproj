#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define array_length(array) (sizeof(array) / sizeof(*(array)))

char *concatenate_strings(const char *const *strings, size_t count) {
	assert(strings != NULL);

	size_t total_length = 0;
	for (size_t i = 0; i < count; i++) {
		total_length += strlen(strings[i]);
	}

	char *total = malloc(total_length + 1);
	if (total == NULL) {
		return NULL;
	}
	total[0] = '\0';

	for (size_t i = 0; i < count; i++) {
		strlcat(total, strings[i], total_length + 1);
	}

	return total;
}
#define concatenate_strings(...)                                                                   \
	concatenate_strings(                                                                           \
		(const char *[]){ __VA_ARGS__ },                                                           \
		array_length(((const char *[]){ __VA_ARGS__ }))                                            \
	)

char *get_cmake_version(void) {
	FILE *file = popen("cmake --version", "r");
	if (file == NULL) {
		return NULL;
	}

	enum {
		MAXIMUM_BUFFER_SIZE = 256
	};
	char buffer[MAXIMUM_BUFFER_SIZE];
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		return NULL;
	}
	buffer[strcspn(buffer, "\n")] = '\0';

	char *cmake_version = strstr(buffer, "version ");
	if (cmake_version == NULL) {
		return NULL;
	}
	cmake_version += strlen("version ");

	if (pclose(file) == -1) {
		return NULL;
	}

	return strdup(cmake_version);
}

int create_project_directory(const char *project_path) {
	assert(project_path != NULL);

	if (mkdir(project_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_subdirectories(const char *project_path) {
	assert(project_path != NULL);

	static const char *subdirectories[] = { "source", "resource", "header", "library" };
	for (size_t i = 0; i < array_length(subdirectories); i++) {
		char *path = concatenate_strings(project_path, "/", subdirectories[i]);
		if (path == NULL) {
			return EXIT_FAILURE;
		}

		if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
			free(path);
			return EXIT_FAILURE;
		}

		free(path);
	}

	return EXIT_SUCCESS;
}
int create_project_cmakelists_txt(const char *project_path) {
	assert(project_path != NULL);

	const char *project_name = strrchr(project_path, '/');
	if (project_name != NULL) {
		project_name++;
	} else {
		project_name = project_path;
	}

	char *cmake_version = get_cmake_version();

	char *path = concatenate_strings(project_path, "/", "CMakeLists.txt");
	if (path == NULL) {
		free(cmake_version);
		return EXIT_FAILURE;
	}

	FILE *file = fopen(path, "w");
	free(path);
	if (file == NULL) {
		free(cmake_version);
		return EXIT_FAILURE;
	}

	if (fprintf(
			file,
			"cmake_minimum_required(VERSION %s)\n"
			"project(%s VERSION 1.0 LANGUAGES C)\n"
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
			"			-Wconversion\n"
			"			-Wunreachable-code\n"
			")\n",
			cmake_version,
			project_name
		) < 0) {
		free(cmake_version);
		return EXIT_FAILURE;
	}

	free(cmake_version);

	if (fclose(file) == EOF) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_cmake_format_py(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", ".cmake-format.py");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	FILE *file = fopen(path, "w");
	free(path);
	if (file == NULL) {
		return EXIT_FAILURE;
	}

	if (fputs(
			"with section(\"format\"):\n"
			"    tab_size = 4\n"
			"    use_tabchars = True\n"
			"    dangle_parens = True\n",
			file
		) < 0) {
		return EXIT_FAILURE;
	}

	if (fclose(file) == EOF) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_cmakelintrc(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", ".cmakelintrc");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	FILE *file = fopen(path, "w");
	free(path);
	if (file == NULL) {
		return EXIT_FAILURE;
	}

	if (fputs("filter=-whitespace/tabs\n", file) < 0) {
		return EXIT_FAILURE;
	}

	if (fclose(file) == EOF) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_source_main_c(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", "source/main.c");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	FILE *file = fopen(path, "w");
	free(path);
	if (file == NULL) {
		return EXIT_FAILURE;
	}

	if (fprintf(
			file,
			"#include <stdio.h>\n"
			"\n"
			"int main(void) {\n"
			"	printf(\"Hello, World!\\n\");\n"
			"}\n"
		) < 0) {
		return EXIT_FAILURE;
	}

	if (fclose(file) == EOF) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_clang_format(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", ".clang-format");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	FILE *file = fopen(path, "w");
	free(path);
	if (file == NULL) {
		return EXIT_FAILURE;
	}

	if (fputs(
			"ColumnLimit: 100\n"
			""
			"UseTab: Always\n"
			"IndentWidth: 4\n"
			"TabWidth: 4\n"
			"\n"
			"AlignAfterOpenBracket: BlockIndent\n"
			"\n"
			"AllowShortBlocksOnASingleLine: Empty\n"
			"AllowShortCaseLabelsOnASingleLine: true\n"
			"AllowShortEnumsOnASingleLine: false\n"
			"AllowShortFunctionsOnASingleLine: Empty\n"
			"AllowShortIfStatementsOnASingleLine: Never\n"
			"AllowShortLoopsOnASingleLine: false\n"
			"\n"
			"IndentCaseLabels: true\n"
			"  \n"
			"Cpp11BracedListStyle: false\n"
			"\n"
			"AllowAllArgumentsOnNextLine: false\n"
			"AllowAllParametersOfDeclarationOnNextLine: false\n"
			"BinPackArguments: false\n"
			"BinPackParameters: false\n"
			"\n"
			"PenaltyReturnTypeOnItsOwnLine: 5000\n",
			file
		) < 0) {
		return EXIT_FAILURE;
	}

	if (fclose(file) == EOF) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_clang_tidy(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", ".clang-tidy");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	FILE *file = fopen(path, "w");
	free(path);
	if (file == NULL) {
		return EXIT_FAILURE;
	}

	if (fputs(
			"Checks: \"\n"
			"  *,\n"
			"  -altera-*,\n"
			"  -llvmlibc-*,\n"
			"  -android-*,\n"
			"  -clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,\n"
			"\"\n",
			file
		) < 0) {
		return EXIT_FAILURE;
	}

	if (fclose(file) == EOF) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int create_project_build(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", "build");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	char *command = concatenate_strings("cmake -S ", project_path, " -B ", path, " > /dev/null");
	free(path);
	if (command == NULL) {
		return EXIT_FAILURE;
	}

	if (system(command) != 0) {
		free(command);
		return EXIT_FAILURE;
	}

	free(command);

	return EXIT_SUCCESS;
}
int create_project_compile_commands_json(const char *project_path) {
	assert(project_path != NULL);

	char *path = concatenate_strings(project_path, "/", "/compile_commands.json");
	if (path == NULL) {
		return EXIT_FAILURE;
	}

	if (symlink("build/compile_commands.json", path) == -1) {
		free(path);
		return EXIT_FAILURE;
	}

	free(path);

	return EXIT_SUCCESS;
}
int create_project(const char *project_path) {
	assert(project_path != NULL);

	static int (*const steps[])(const char *) = {
		create_project_directory,	   create_project_subdirectories,
		create_project_cmakelists_txt, create_project_cmake_format_py,
		create_project_cmakelintrc,	   create_project_source_main_c,
		create_project_clang_format,   create_project_clang_tidy,
		create_project_build,		   create_project_compile_commands_json,
	};
	for (size_t i = 0; i < array_length(steps); i++) {
		if (steps[i](project_path) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		return EXIT_FAILURE;
	}

	const char *project_path = argv[1];
	if (project_path[0] == '\0') {
		project_path = ".";
	}

	if (create_project(project_path) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
