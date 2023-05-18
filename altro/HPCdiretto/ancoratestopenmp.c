#include <stdio.h>
#include <omp.h>

#define SIZE 100

void game_of_life(int matrix[SIZE][SIZE]) {
    // ...
    // Resto del codice del gioco della vita
    // ...
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
    for (int iter = 0; iter < 100; iter++) {
        game_of_life(matrix);
    }
    
    double end_time = omp_get_wtime(); // Tempo di fine
    double elapsed_time = end_time - start_time; // Tempo trascorso
    
    printf("Tempo di esecuzione: %.6f secondi\n", elapsed_time);
    
    return 0;
}
