#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100
#define ITERATIONS 100

int count_neighbors(int matrix[SIZE][SIZE], int row, int col) {
    int count = 0;
    int i, j;
    
    for (i = row - 1; i <= row + 1; i++) {
        for (j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                if (i != row || j != col) {
                    count += matrix[i][j];
                }
            }
        }
    }
    
    return count;
}

void game_of_life(int matrix[SIZE][SIZE]) {
    int new_matrix[SIZE][SIZE];
    int i, j;
    
    #pragma omp parallel for private(i, j) shared(matrix, new_matrix)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            int neighbors = count_neighbors(matrix, i, j);
            
            if (matrix[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    new_matrix[i][j] = 0;
                } else {
                    new_matrix[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    new_matrix[i][j] = 1;
                } else {
                    new_matrix[i][j] = 0;
                }
            }
        }
    }
    
    // Copia il nuovo stato nella matrice originale
    #pragma omp parallel for private(i, j)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            matrix[i][j] = new_matrix[i][j];
        }
    }
}

int main() {
    int matrix[SIZE][SIZE];
    int i, j;
    
    // Inizializza la matrice con valori casuali
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 2;
        }
    }
    
    double start_time = omp_get_wtime(); // Tempo di inizio
    
    // Esegui il gioco della vita per un certo numero di iterazioni
    for (int iter = 0; iter < ITERATIONS; iter++) {
        game_of_life(matrix);
    }
    
    double end_time = omp_get_wtime(); // Tempo di fine
    double elapsed_time = end_time - start_time; // Tempo trascorso
    
    printf("Tempo di esecuzione: %.6f secondi\n", elapsed_time);
    
    return 0;
}
