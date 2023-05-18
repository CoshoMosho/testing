#include <stdio.h>
#include <stdint.h>

#define ROWS 100 // numero di righe dell'immagine
#define COLS 100 // numero di colonne dell'immagine
#define MAXVAL 256

int main() {
  FILE *fp;
  uint_fast8_t f  = 123;
  fp = fopen("image.pgm", "w"); // apriamo il file in scrittura
  
  fprintf(fp, "P2\n"); // scriviamo l'intestazione del file PBM
  fprintf(fp, "%d %d\n %d\n", COLS, ROWS, MAXVAL); // scriviamo le dimensioni dell'immagine
  
  // creiamo l'immagine con una riga di pixel bianchi e una di neri
  for(int i = 0; i < ROWS; i++) {
    for(int j = 0; j < COLS; j++) {
      if((i/10)%2 == 0) { // riga pari = bianco
        fprintf(fp, "255 ");
      } else { // riga dispari = nero
        fprintf(fp, "0 ");
      }
    }
    fprintf(fp, "\n");
  }
  
  fclose(fp); // chiudiamo il file
  return 0;
}
