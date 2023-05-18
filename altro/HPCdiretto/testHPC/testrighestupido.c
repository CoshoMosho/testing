#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROC 2   // Numero di processi nella lista
#define NUM_ITER 100 // Numero di iterazioni per processo

int main()
{
    int proc[NUM_PROC] = {17, 16}; // Elenco dei processi nella lista
    int iter_count[NUM_PROC] = {0}; // Contatore delle iterazioni per ogni processo
    char line[100]; // Buffer per la lettura di ogni riga

    // Apertura del file contenente la lista
    FILE *fp = fopen("list.txt", "r");
    if (fp == NULL) {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }

    // Lettura di ogni riga del file
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Controllo se la riga contiene l'informazione di una iterazione
        if (strstr(line, "iterazione") != NULL) {
            // Controllo di quale processo si tratta
            for (int i = 0; i < NUM_PROC; i++) {
                if (strstr(line, "processo ") != NULL && proc[i] == atoi(strstr(line, "processo ") + 9)) {
                    // Controllo che l'iterazione corrisponda al contatore del processo
                    if (strstr(line, "iterazione ") != NULL && iter_count[i] + 65 == atoi(strstr(line, "iterazione ") + 11)) {
                        // Aggiornamento del contatore per il processo corrente
                        iter_count[i]++;
                        break;
                    }
                }
            }
        }
    }

    // Controllo che ogni processo abbia esattamente 100 iterazioni
    for (int i = 0; i < NUM_PROC; i++) {
        if (iter_count[i] != NUM_ITER) {
            printf("Il processo %d non ha esattamente %d iterazioni\n", proc[i], NUM_ITER);
        }
    }

    // Chiusura del file
    fclose(fp);

    return 0;
}
