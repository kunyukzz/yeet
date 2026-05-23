#include "yeet.h"

int main(void)
{
    yeet_init();
    yeet_compiler_use("CLANG", "C11");
    yeet_compiler_flags("-g", "-Wall", "-Wextra", NULL);
    yeet_compiler_linkers("-lGL", "-lm", NULL);

    yeet_target("lalala", "test_dir/binary", "test_dir/obj");
    yeet_source_all("test_dir/src");

    yeet_build();
}
