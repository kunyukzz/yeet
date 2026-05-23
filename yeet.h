// Your Embedded Executable Tool

#include <ctype.h>  // tolower
#include <stdarg.h> // va_arg, va_list, va_start, va_end
#include <stdio.h>  // snprintf
#include <stdlib.h> // calloc
#include <string.h> // strncpy

#define MAX_FLAGS 64
#define MAX_FLAG_LEN 32
#define MAX_LINKERS 16
#define MAX_LINKER_LEN 16

typedef struct {
    char compiler[32];
    char version[16];
    char flags[MAX_FLAGS][MAX_FLAG_LEN];
    int flag_count;
    char linkers[MAX_LINKERS][MAX_LINKER_LEN];
    int linker_count;
} compiler_t;

typedef struct {
    compiler_t compiler_cfg;
} yeet_builder;

static yeet_builder *g_yeet = {0};

// public API
void yeet_init(void);

void yeet_compiler_use(const char *compiler, const char *c_version);
void yeet_compiler_flags(const char *flag, ...);
void yeet_compiler_linkers(const char *linker, ...);

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

static inline void _add_flag(const char *flag)
{
    if (!g_yeet || !flag) return;

    if (g_yeet->compiler_cfg.flag_count >= MAX_FLAGS)
    {
        fprintf(stderr, "Too many compiler flags\n");
        exit(1);
    }

    int idx = g_yeet->compiler_cfg.flag_count;
    strncpy(g_yeet->compiler_cfg.flags[idx], flag, MAX_FLAG_LEN - 1);
    g_yeet->compiler_cfg.flags[idx][MAX_FLAG_LEN - 1] = '\0';
    g_yeet->compiler_cfg.flag_count++;
}

static inline void _add_linker(const char *linker)
{
    if (!g_yeet || !linker) return;

    if (g_yeet->compiler_cfg.linker_count >= MAX_LINKERS)
    {
        fprintf(stderr, "Too many linker\n");
        exit(1);
    }

    int idx = g_yeet->compiler_cfg.linker_count;
    strncpy(g_yeet->compiler_cfg.linkers[idx], linker, MAX_LINKER_LEN - 1);
    g_yeet->compiler_cfg.linkers[idx][MAX_LINKER_LEN - 1] = '\0';
    g_yeet->compiler_cfg.linker_count++;
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

    free(g_yeet);
    g_yeet = NULL;
}
