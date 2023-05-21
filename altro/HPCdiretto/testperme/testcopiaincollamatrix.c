#include <stdio.h>
#include <omp.h>
#define SIZE 1000

void riempiMatrice(int matrice[SIZE][SIZE], int righe, int colonne) {
    int numero = 1; // Il primo numero da inserire nella matrice
    
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            matrice[i][j] = numero; // Inserisce il numero corrente nella cella della matrice
            numero++; // Incrementa il numero successivo
        }
    }
}

void moltiplicaMatrice(int matrice1[SIZE][SIZE], int matrice2[SIZE][SIZE], int righe, int colonne) {
    
    int id;
    #pragma omp parallel 
    {
    #pragma omp for    
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            id = omp_get_thread_num();
            printf("id thread is : %d",id);
            matrice2[i][j] = matrice1[i][j] * 10 * 10 ; // Moltiplica l'elemento corrente per 10 e lo assegna alla seconda matrice
            printf("thread numero %d \n",id);
        }
    }
    }
    


    printf("\nMatrice 2:\n");
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("thread numero %d opera sul %d \n",id, matrice2[i][j]);
        }
        printf("\n");
    }
}

int main() {


    double start_time = omp_get_wtime(); // Tempo di inizio
    
    int max_threads = omp_get_max_threads();
    printf(" n of threads is %d", max_threads);
    int righe, colonne;
    printf("Inserisci il numero di righe della matrice: ");
    //scanf("%d", &righe);
    printf("Inserisci il numero di colonne della matrice: ");
    //scanf("%d", &colonne);
    righe = SIZE;
    colonne = SIZE;
    int matrice1[righe][colonne];
    int matrice2[righe][colonne];

    riempiMatrice(matrice1, righe, colonne);

    moltiplicaMatrice(matrice1, matrice2, righe, colonne);

    printf("Matrice 1:\n");
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("%d ", matrice1[i][j]);
        }
        printf("\n");
    }

    double end_time = omp_get_wtime(); // Tempo di fine
    double elapsed_time = end_time - start_time; // Tempo trascorso
    
    printf("Tempo di esecuzione: %.6f secondi\n", elapsed_time);

    return 0;
}
