#include <stdio.h>
#include <stdlib.h>

#define WIDTH 512 // larghezza dell'immagine
#define HEIGHT 512 // altezza dell'immagine

int main() {
  FILE *file = fopen("immagine.pgm", "wb"); // apriamo il file in modalità binaria
  fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT); // scriviamo l'header PGM
  
  // scriviamo i dati dell'immagine
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (x < WIDTH / 2) { // metà sinistra
        fputc(255, file); // bianco
      } else { // metà destra
        fputc(0, file); // nero
      }
    }
  }
  
  fclose(file); // chiudiamo il file
  return 0;
}
