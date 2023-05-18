// #include <stdio.h>

// int main(){

//     int memoryPoint = 3;
//     stampaTutto(&memoryPoint);
//     printf(" -- test ended --");
// }

// void stampaTutto(int *memoryPoint){
//     printf("variable add is: %p ---",memoryPoint);
//      printf("variable is: %d ---",*memoryPoint);
// }

// void stampa()
// {
//     printf("variable add is: %p ---",memoryPoint);
// }

// #include <stdio.h>

// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         printf("Usage: %s <address>\n", argv[0]);
//         return 1;
//     }
//     void *addr = (void *) strtol(argv[1], NULL, 16);
//     unsigned char *p = (unsigned char *) addr;
//     printf("Values of 16 bytes starting from address %p:\n", addr);
//     for (int i = 0; i < 16; i++) {
//         printf("%02x ", *(p + i));
//     }
//     printf("\n");
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 100



int main(int argc, char *argv[]) {
    // if (argc < 2) {
    //     printf("Usage: %s <file>\n", argv[0]);
    //     return 1;
    // }
    // char *filename = argv[1];
    char *filename = "te.txt";
    while (1) {
        FILE *fp = fopen(filename, "r");
        if (!fp) {
            printf("Could not open file '%s'\n", filename);
            return 1;
        }
        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, fp)) {
            void *addr = (void *) strtol(line, NULL, 16);
            // unsigned char *p = (unsigned char *) addr;
            printf("Address %p: ", addr);
            char *p = (char *) addr;
            for (int i = 0; i < 8; i++) {
                // printf("%02x ", *(p + i));
                 printf("%c ", *(p + i));
            }
            printf("\n");

        printf("\n ");
        for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*p & (0x80 >> j)) != 0) {
                printf("1 ");
            } else {
                printf("0 ");
            }
        }
        printf("\n ");
        p++;
    }
    printf("\n");
        }
        fclose(fp);
        sleep(5);
    }
    return 0;
}

// #include <stdio.h>

// int main() {
//     char *p = (char *) 0x555555556004; // assegna il puntatore all'indirizzo di memoria
//     printf("Address %p: %c %c %c %c %c %c %c %c\n",
//            p, *p, *(p+1), *(p+2), *(p+3), *(p+4), *(p+5), *(p+6), *(p+7));
//     return 0;
// }



