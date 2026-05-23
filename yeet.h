// Your Embedded Executable Tool

#include <ctype.h>    // tolower
#include <stdarg.h>   // va_arg, va_list, va_start, va_end
#include <stdio.h>    // snprintf
#include <stdlib.h>   // calloc
#include <string.h>   // strncpy
#include <sys/stat.h> // stat, mkdir

#define MAX_FLAGS 64
#define MAX_FLAG_LEN 32
#define MAX_LINKERS 16
#define MAX_LINKER_LEN 16

#define MAX_PATH_LEN 128
#define MAX_SRC 128

typedef struct {
    char compiler[32];
    char version[16];
    char flags[MAX_FLAGS][MAX_FLAG_LEN];
    int flag_count;
    char linkers[MAX_LINKERS][MAX_LINKER_LEN];
    int linker_count;
} compiler_t;

typedef struct {
    char binary[MAX_PATH_LEN];
    char object[MAX_PATH_LEN];
} path_build_t;

typedef struct {
    char directory[64];
    char exclude[MAX_PATH_LEN];
    char storage[MAX_SRC][MAX_PATH_LEN];
    char *files[MAX_SRC]; // for multi-build in single run
    int count;
} source_t;

typedef struct {
    compiler_t compiler_cfg;
    path_build_t target_path;
    char target_name[16];
    source_t sources;
} yeet_builder;

static yeet_builder *g_yeet = {0};

// public API
void yeet_init(void);

void yeet_compiler_use(const char *compiler, const char *c_version);
void yeet_compiler_flags(const char *flag, ...);
void yeet_compiler_linkers(const char *linker, ...);

void yeet_target(const char *name, const char *binary_path, const char *object_path);
void yeet_source_all(const char *source_path);

void yeet_build(void);

// helper
static inline void _str_lower(char *str)
{
    while (*str)
    {
        *str = (char)tolower((unsigned char)*str);
        str++;
    }
}

static inline void _str_copy(char *dest, const char *src, size_t size)
{
    if (!dest || !src || size == 0) return;

    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

static void _add_flag(const char *flag)
{
    if (!g_yeet || !flag) return;

    if (g_yeet->compiler_cfg.flag_count >= MAX_FLAGS)
    {
        fprintf(stderr, "Too many compiler flags\n");
        exit(1);
    }

    int idx = g_yeet->compiler_cfg.flag_count;

    _str_copy(g_yeet->compiler_cfg.flags[idx], flag, MAX_FLAG_LEN);

    g_yeet->compiler_cfg.flag_count++;
}

static void _add_linker(const char *linker)
{
    if (!g_yeet || !linker) return;

    if (g_yeet->compiler_cfg.linker_count >= MAX_LINKERS)
    {
        fprintf(stderr, "Too many linker\n");
        exit(1);
    }

    int idx = g_yeet->compiler_cfg.linker_count;

    _str_copy(g_yeet->compiler_cfg.linkers[idx], linker, MAX_LINKER_LEN);

    g_yeet->compiler_cfg.linker_count++;
}

static int _create_dir(const char *path)
{
    struct stat st = {0};
    if (stat(path, &st) == -1) return mkdir(path, 0777);
    return 0;
}

static void _set_source(const char *path)
{
    if (!path || strcmp(path, ".") == 0 || strcmp(path, "./") == 0)
    {
        fprintf(stderr,
                "(\"%s\") isn't allowed.\n"
                "Source files must live in a directory (e.g. src/).\n",
                path ? path : "(null)");
        exit(1);
    }

    _str_copy(g_yeet->sources.directory, path, MAX_PATH_LEN);

    for (int i = 0; i < g_yeet->sources.count; ++i)
    {
        free(g_yeet->sources.files[i]);
    }

    g_yeet->sources.count = 0;
}

/* CORE API IMPLEMENTATION */

void yeet_init(void)
{
    if (!g_yeet)
    {
        g_yeet = calloc(1, sizeof(yeet_builder));
        if (!g_yeet)
        {
            fprintf(stderr, "Failed to initialize yeet\n");
            exit(1);
        }
    }
}

// compiler logic
void yeet_compiler_use(const char *compiler, const char *c_version)
{
    strncpy(g_yeet->compiler_cfg.compiler, compiler, sizeof(g_yeet->compiler_cfg.compiler) - 1);
    g_yeet->compiler_cfg.compiler[sizeof(g_yeet->compiler_cfg.compiler) - 1] = '\0';
    _str_lower(g_yeet->compiler_cfg.compiler);

    snprintf(g_yeet->compiler_cfg.version, sizeof(g_yeet->compiler_cfg.version), "-std=%s", c_version);
    _str_lower(g_yeet->compiler_cfg.version);
}

void yeet_compiler_flags(const char *flag, ...)
{
    if (!flag) return;

    va_list args;
    va_start(args, flag);

    const char *current = flag;

    while (current)
    {
        _add_flag(current);
        current = va_arg(args, const char *);
    }

    va_end(args);
}

void yeet_compiler_linkers(const char *linker, ...)
{
    if (!linker) return;

    va_list args;
    va_start(args, linker);

    const char *current = linker;

    while (current)
    {
        _add_linker(current);
        current = va_arg(args, const char *);
    }

    va_end(args);
}

void yeet_target(const char *name, const char *binary_path, const char *object_path)
{
    _str_copy(g_yeet->target_name, name, sizeof(g_yeet->target_name));

    _str_copy(g_yeet->target_path.binary, binary_path, sizeof(g_yeet->target_path.binary));
    _create_dir(binary_path);

    _str_copy(g_yeet->target_path.object, object_path, sizeof(g_yeet->target_path.object));
    _create_dir(object_path);
}

void yeet_source_all(const char *source_path)
{
    if (!source_path) return;

    _set_source(source_path);

    g_yeet->sources.exclude[0] = '\0';
}

// actual building logic
void yeet_build(void)
{
    // NOT YET IMPLEMENTED!!
    printf("Compiler: %s, Version: %s\n", g_yeet->compiler_cfg.compiler, g_yeet->compiler_cfg.version);

    printf("Flags (%d):\n", g_yeet->compiler_cfg.flag_count);
    for (int i = 0; i < g_yeet->compiler_cfg.flag_count; ++i)
    {
        printf(" [%d] %s\n", i, g_yeet->compiler_cfg.flags[i]);
    }

    printf("Linkers (%d):\n", g_yeet->compiler_cfg.linker_count);
    for (int i = 0; i < g_yeet->compiler_cfg.linker_count; ++i)
    {
        printf(" [%d] %s\n", i, g_yeet->compiler_cfg.linkers[i]);
    }

    printf("Name: %s\n", g_yeet->target_name);
    printf("Binary Path: %s, object Path: %s\n", g_yeet->target_path.binary, g_yeet->target_path.binary);

    printf("Source Path: %s\n", g_yeet->sources.directory);
    for (int i = 0; i < g_yeet->sources.count; ++i)
    {
        printf(" [%d] %s\n", i, *g_yeet->sources.files);
    }

    free(g_yeet);
    g_yeet = NULL;
}
