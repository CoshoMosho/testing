#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s -g INT\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "-g") != 0) {
        printf("Error: First argument must be -g\n");
        return 1;
    }
    int num = atoi(argv[2]);
    if (num < 0 || num > 112) {
        printf("Error: Second argument must be an integer between 0 and 112\n");
        return 1;
    }

    #pragma omp_in_parallel

    printf("Params are correct: -g %d\n", num);
    return 0;
}
