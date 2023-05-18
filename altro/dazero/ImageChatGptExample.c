#include <stdio.h>

int main()
{
    // Definizione delle dimensioni dell'immagine
    int width = 2;
    int height = 2;
    int max_val = 255;

    // Apertura del file per la scrittura
    FILE* file = fopen("image.pgm", "wb");
    srand(time(NULL));
    // Scrittura dell'header del file PBM
    fprintf(file, "P5\n%d %d\n%d\n", width, height, max_val);

    // Scrittura dei pixel dell'immagine
    unsigned char pixel = 0x00;
    unsigned char pixelvoid = ~(pixel);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Generazione di un valore casuale per il pixel
            pixel = (unsigned char)(rand() % 2)*255;
            printf("%u ",pixel);
            
            // Scrittura del valore del pixel sul file
            fwrite(&pixelvoid, sizeof(pixel), 1, file);
        }
        printf("\n");
    }

    // Chiusura del file
    fclose(file);

    return 0;
}
