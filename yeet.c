#define YEET_IMPL
#include "yeet.h"

int main(void)
{
    yeet_init();
    yeet_compiler_use("CLANG", "C11");
    yeet_compiler_flags("-g", "-Wall", "-Wextra", NULL);
    yeet_compiler_linkers("-lGL", "-lm", NULL);

    yeet_target("lalala", "test_dir/binary", "test_dir/obj");
    yeet_source_all("test_dir/src");
    // yeet_source_exclude("test_dir", "test");

    // const char *my_file[] = {"a", "b", "c", "d", NULL};
    // yeet_source_spesific("test_dir/src", my_file);

    yeet_build();
}
