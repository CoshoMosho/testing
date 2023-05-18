#include <stdio.h>
#include <omp.h>

#define SIZE 100

void game_of_life(int matrix[SIZE][SIZE]) {
    int i, j;
    int new_matrix[SIZE][SIZE];
    
    #pragma omp parallel for private(i, j) shared(matrix, new_matrix)
    for (i = 1; i < SIZE - 1; i++) {
        for (j = 1; j < SIZE - 1; j++) {
            int neighbors = 0;
            
            // Calcolo il numero di vicini vivi
            neighbors += matrix[i-1][j-1];
            neighbors += matrix[i-1][j];
            neighbors += matrix[i-1][j+1];
            neighbors += matrix[i][j-1];
            neighbors += matrix[i][j+1];
            neighbors += matrix[i+1][j-1];
            neighbors += matrix[i+1][j];
            neighbors += matrix[i+1][j+1];
            
            // Aggiorno lo stato delle celle
            if (matrix[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3)
                    new_matrix[i][j] = 0; // La cella muore
                else
                    new_matrix[i][j] = 1; // La cella sopravvive
            } else {
                if (neighbors == 3)
                    new_matrix[i][j] = 1; // Una nuova cella viene generata
                else
                    new_matrix[i][j] = 0; // La cella rimane morta
            }
        }
    }
    
    // Copio la nuova matrice nel posto della matrice originale
    #pragma omp parallel for private(i, j) shared(matrix, new_matrix)
    for (i = 1; i < SIZE - 1; i++) {
        for (j = 1; j < SIZE - 1; j++) {
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
    
    // Esegui il gioco della vita per un certo numero di iterazioni
    for (int iter = 0; iter < 100; iter++) {
        game_of_life(matrix);
    }
    
    return 0;
}
