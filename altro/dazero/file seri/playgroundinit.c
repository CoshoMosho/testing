#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#define XWIDTH 256
#define YWIDTH 256
#define MAXVAL 255


void stampaCells(char *cells, int row_length, int n_rows)
{
  int ele = 0;
  for (int j = 0; j < n_rows; j++)
  {
    int multRiga = j * row_length;
    for (int i = 0; i < row_length; i++)
    {
      ele++;
      if (cells[multRiga + i] == 255)
      {
        printf("1 ");
      }
      else
      {
        printf("0 ");
      }
    }
    printf("\n");
  }
  printf("stampa elements");
  printf("%d elements",ele);
  printf("\n");
}


// void read_pgm_image( void **image, int *xsize, int *ysize,  int *maxval,const char *image_name)
// /*
//  * image        : a pointer to the pointer that will contain the image
//  * maxval       : a pointer to the int that will store the maximum intensity in the image
//  * xsize, ysize : pointers to the x and y sizes
//  * image_name   : the name of the file to be read
//  *
//  */
// {
//   FILE* image_file; 
//   image_file = fopen(image_name, "r"); 

//   *image = NULL;
//   *xsize = *ysize = *maxval = 0;
  
//   char    MagicN[2];
//   char   *line = NULL;
//   size_t  k, n = 0;
  
//   // get the Magic Number
//   k = fscanf(image_file, "%2s%*c", MagicN );

//   // skip all the comments
//   k = getline( &line, &n, image_file);
//   while ( (k > 0) && (line[0]=='#') )
//     k = getline( &line, &n, image_file);

//   if (k > 0)
//     {
//       k = sscanf(line, "%d%*c%d%*c%d%*c", xsize, ysize, maxval);
//       if ( k < 3 )
// 	fscanf(image_file, "%d%*c", maxval);
//     }
//   else
//     {
//       *maxval = -1;         // this is the signal that there was an I/O error
// 			    // while reading the image header
//       free( line );
//       return;
//     }
//   free( line );
  
//   int color_depth = 1 + ( *maxval > 255 );
//   unsigned int size = *xsize * *ysize * color_depth;
  
//   if ( (*image = (char*)malloc( size )) == NULL )
//     {
//       fclose(image_file);
//       *maxval = -2;         // this is the signal that memory was insufficient
//       *xsize  = 0;
//       *ysize  = 0;
//       return;
//     }
  
//   if ( fread( *image, 1, size, image_file) != size )
//     {
//       free( image );
//       image   = NULL;
//       *maxval = -3;         // this is the signal that there was an i/o error
//       *xsize  = 0;
//       *ysize  = 0;
//     }  

//   fclose(image_file);
//   return;
// }


void write_pgm_image( void *image,  int xsize, int ysize, int maxval, const char *image_name)
/*
 * image        : a pointer to the memory region that contains the image
 * maxval       : either 255 or 65536
 * xsize, ysize : x and y dimensions of the image
 * image_name   : the name of the file to be written
 *
 */
{
  FILE* image_file; 
  image_file = fopen(image_name, "wb"); 
  
  // Writing header
  // The header's format is as follows, all in ASCII.
  // "whitespace" is either a blank or a TAB or a CF or a LF
  // - The Magic Number (see below the magic numbers)
  // - the image's width
  // - the height
  // - a white space
  // - the image's height
  // - a whitespace
  // - the maximum color value, which must be between 0 and 65535
  //
  // if he maximum color value is in the range [0-255], then
  // a pixel will be expressed by a single byte; if the maximum is
  // larger than 255, then 2 bytes will be needed for each pixel
  //

  int color_depth = 1 + ( maxval > 255 );

  fprintf(image_file, "P5\n# generated by\n# vicari \n%d %d\n%d\n", xsize, ysize, maxval);
  
  // Writing file
  fwrite( image, 1, xsize*ysize*color_depth, image_file);  
  //stampaCells(image,xsize, ysize);
  fclose(image_file); 
  return ;

  /* ---------------------------------------------------------------
     TYPE    MAGIC NUM     EXTENSION   COLOR RANGE
           ASCII  BINARY
     PBM   P1     P4       .pbm        [0-1]
     PGM   P2     P5       .pgm        [0-255]
     PPM   P3     P6       .ppm        [0-2^16[
  
  ------------------------------------------------------------------ */
}

char* generate_random_binary_sequence(int length){

  unsigned char *random_values = calloc(length, sizeof(char)); // array che conterrà i valori casuali generati
  srand(time(NULL)); // inizializza il generatore di numeri casuali
  for (int i = 0; i < length; i++)
  {
    // genera un numero casuale compreso tra 0 e 1
    int random_number = rand() % 2;
    // converte il numero casuale in un char
    random_values[i] = (char)(random_number*255); 
  }
  return random_values;
}


char* random_fill_matrix(int strict_length, int strict_width, unsigned char* cells){
   int length =strict_length*strict_width;
   unsigned char *random_values = generate_random_binary_sequence(length);
   //stampaCells(random_values,strict_length,strict_width);
  for (int i = 0; i < length; i++)
  {
     *(cells+i) = *random_values;
     //printf("%d ",*(cells+i) );
    random_values++;
  }
}



int main( int argc, char **argv ) 
{ 
    int xsize      = XWIDTH;
    int ysize      = YWIDTH;
    int maxval     = MAXVAL;

    xsize      = 1000;
    ysize      = 1000;
    maxval     = MAXVAL;

    unsigned char *cells = calloc(xsize*ysize, sizeof(char));
    random_fill_matrix(xsize,ysize, cells);
    //stampaCells(cells,xsize,ysize);
    write_pgm_image(cells,xsize,ysize,maxval,"playground.pgm");
    // printf("read now");
    // read_pgm_image( &cells, &xsize, &ysize, &maxval,"bananajoeY.pgm");
    // write_pgm_image(cells,xsize,ysize,maxval,"imagecopiaincolla.pgm");
    free(cells);
}


// int main(int argc, int *argv[])
// {
//     int dimension = 15 * 11;
//     char random_values[dimension]; // array che conterrà i valori casuali generati

//     srand(time(NULL)); // inizializza il generatore di numeri casuali

//     for (int i = 0; i < dimension; i++)
//     {
//         // genera un numero casuale compreso tra 0 e 1
//         int random_number = rand() % 2;

//         // converte il numero casuale in un char
//         random_values[i] = (char)random_number;
//     }

//     // stampa i valori casuali generati
//     for (int i = 0; i < dimension; i++)
//     {
//         printf("%d ", random_values[i]);
//     }
//     printf("\n");
// }