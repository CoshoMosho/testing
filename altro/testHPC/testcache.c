#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define CACHE_SIZE (512 * MB) // L3 cache size of AMD EPYC 7H12
#define ARRAY_SIZE (2 * GB)

int main() {
  int *array = (int *)malloc(ARRAY_SIZE);
  if (array == NULL) {
    perror("malloc failed");
    return -1;
  }

  clock_t start = clock();

  for (int i = 0; i < ARRAY_SIZE / sizeof(int); i += CACHE_SIZE / sizeof(int)) {
    array[i]++;
  }

  clock_t end = clock();
  double time_elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("Time elapsed: %f seconds\n", time_elapsed);
  return 0;
}
