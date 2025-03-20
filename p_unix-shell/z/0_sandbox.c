#include <stdio.h>

int main(int argc, char *argv[]) {
    // Calculate the size of the argv array
    size_t size_of_argv = 0;

    for (int i = 0; i < argc; ++i) {
        size_of_argv += sizeof(argv[i]);
    }

    printf("Size of argv array: %lu bytes\n", size_of_argv);

    return 0;
}
