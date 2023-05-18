#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 
#include <stdbool.h>

 //-impostazioni 
 // Randomisation seed
unsigned int seed;
unsigned int height = 400;
unsigned int width = 400;

   
//++++++(long o int?)
unsigned int numberOfGenerations;

//-altre impostazioni 

//matrice game of life
unsigned char* cells;
unsigned char* temp_cells;
unsigned int width;
unsigned int height;


int main(int argc, int* argv[])
{
   

    //metodo per prendere i valori da terminale e controllarli

    //genero la matrice iniziale
    init(&cells);


    //modifiche ai passi successivi

    bool quitWhile = false;


    printf("mah %d",quitWhile);

    //stampa configurazione
}

void setCell(unsigned int x, unsigned int h){

}

int init(char* cells)
{
    unsigned char x,y,init_length;
    seed = (unsigned)time(NULL);

    printf("Initializing...");
    
   srand(seed);
	init_length = (width * height) / 2;
	do
	{
		x = rand() % (width - 1);
		y = rand() % (height - 1);
		// if (CellState(x, y) == 0)
        // {
            // SetCell(x, y);
            printf("setcellstate");
        // }
	} while (--init_length);
    

}