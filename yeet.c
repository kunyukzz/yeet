#include "yeet.h"

int main(void)
{
    yeet_init();
    yeet_compiler_use("CLANG", "C11");
    yeet_compiler_flags("-g", "-Wall", "-Wextra", NULL);
    yeet_compiler_linkers("-lGL", "-lm", NULL);
    yeet_build();
}
