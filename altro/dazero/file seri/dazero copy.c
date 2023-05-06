#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//-impostazioni

#define IMAGE_NAME "playground.pgm"
#define MAX_VAL_PIXEL 255

void stampaCells(unsigned char *cells, int row_length, int n_rows)
{
  for (int j = 0; j < n_rows; j++)
  {
    int multRiga = j * row_length;
    for (int i = 0; i < row_length; i++)
    {
      if (cells[multRiga + i] == 255)
      {
        printf("0 ");
      }
      else
      {
        printf("1 ");
      }
    }
    printf("\n");
  }
  printf("\n");
}

unsigned char *read_pgm_image(int *xsize, int *ysize, int *maxval, const char *image_name)
/*
 * image        : a pointer to the pointer that will contain the image
 * maxval       : a pointer to the int that will store the maximum intensity in the image
 * xsize, ysize : pointers to the x and y sizes
 * image_name   : the name of the file to be read
 *
 */
{
  FILE *image_file;
  image_file = fopen(image_name, "r");

  unsigned char *image = NULL;
  *xsize = *ysize = *maxval = 0;

  char MagicN[2];
  char *line = NULL;
  size_t k, n = 0;

  // get the Magic Number
  k = fscanf(image_file, "%2s%*c", MagicN);

  // skip all the comments
  k = getline(&line, &n, image_file);
  while ((k > 0) && (line[0] == '#'))
    k = getline(&line, &n, image_file);

  if (k > 0)
  {
    k = sscanf(line, "%d%*c%d%*c%d%*c", xsize, ysize, maxval);
    if (k < 3)
      fscanf(image_file, "%d%*c", maxval);
  }
  else
  {
    *maxval = -1; // this is the signal that there was an I/O error
                  // while reading the image header
    free(line);
    return image;
  }
  free(line);

  int color_depth = 1 + (*maxval > 255);
  unsigned int size = *xsize * *ysize * color_depth;

  if ((image = (unsigned char *)malloc(size)) == NULL)
  {
    fclose(image_file);
    *maxval = -2; // this is the signal that memory was insufficient
    *xsize = 0;
    *ysize = 0;
    return;
  }

  if (fread(image, 1, size, image_file) != size)
  {
    free(image);
    image = NULL;
    *maxval = -3; // this is the signal that there was an i/o error
    *xsize = 0;
    *ysize = 0;
  }

  fclose(image_file);
  return image;
}

void write_pgm_image(void *image, int xsize, int ysize, int maxval, const char *image_name)
/*
 * image        : a pointer to the memory region that contains the image
 * maxval       : either 255 or 65536
 * xsize, ysize : x and y dimensions of the image
 * image_name   : the name of the file to be written
 *
 */
{
  FILE *image_file;
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

  int color_depth = 1 + (maxval > 255);

  fprintf(image_file, "P5\n# generated by\n# vicari \n%d %d\n%d\n", xsize, ysize, maxval);

  // Writing file
  fwrite(image, 1, xsize * ysize * color_depth, image_file);
  // stampaCells(image, xsize, ysize);
  fclose(image_file);
  //CONTROLLARE, segmentation fault??
  //free(image_file);
  return;

  /* ---------------------------------------------------------------
     TYPE    MAGIC NUM     EXTENSION   COLOR RANGE
           ASCII  BINARY
     PBM   P1     P4       .pbm        [0-1]
     PGM   P2     P5       .pgm        [0-255]
     PPM   P3     P6       .ppm        [0-2^16[

  ------------------------------------------------------------------ */
}

// char *init(const char *image_name)
// {
//   int xsize      = 0;
//   int ysize      = 0;
//   int maxval     = 0;
//   unsigned char *cells;
//   read_pgm_image( &cells, &xsize, &ysize, &maxval,IMAGE_NAME);
//   cells = calloc(xsize*ysize*2, sizeof(char));
//   return cells;
// }

void static_evolution()
{
}
void ordered_evolution()
{
}

// FIXARE RANDOM
void fill_borders_old(unsigned char *pointer_to_other_cells, unsigned char *borders, int row_length, int n_rows)
{

  // stampaCells(pointer_to_other_cells, row_length, n_rows);

  unsigned char *fill_leftup_up_pointer = pointer_to_other_cells;

  *fill_leftup_up_pointer = *(borders + row_length - 3);
  fill_leftup_up_pointer++;
  for (int f = 0; f < row_length - 2; f++) // up left to up right
  {
    *fill_leftup_up_pointer = *borders;
    fill_leftup_up_pointer++;
    borders++;
  }
  *fill_leftup_up_pointer = *(borders - row_length + 2);
  fill_leftup_up_pointer++;
  return;
  for (int f = 0; f < n_rows - 2; f++) // up left 2nd row to down left
  {
    *fill_leftup_up_pointer = *borders;
    fill_leftup_up_pointer = fill_leftup_up_pointer + row_length;
    borders++;
  }
  *fill_leftup_up_pointer = *(borders - n_rows + 2);
  fill_leftup_up_pointer++;
  for (int f = 0; f < row_length - 2; f++) // down left to down right
  {
    *fill_leftup_up_pointer = *borders;
    fill_leftup_up_pointer++;
    borders++;
  }
  *fill_leftup_up_pointer = *(borders - row_length + 2);
  fill_leftup_up_pointer = fill_leftup_up_pointer - row_length;
  for (int f = 0; f < n_rows - 2; f++) // down right to up right
  {
    *fill_leftup_up_pointer = *borders;
    fill_leftup_up_pointer = fill_leftup_up_pointer - row_length;
    borders++;
  }

  //printf("\n");
  // stampaCells(pointer_to_other_cells, row_length, n_rows);
}

void fill_borders(unsigned char *pointer_to_other_cells, unsigned char *borders, int row_length, int n_rows)
{
  for (int i = 0; i < row_length; i++)
  {
    *pointer_to_other_cells = *borders;
    pointer_to_other_cells++;
    borders++;
  }
  for (int i = 0; i < n_rows - 2; i++)
  {
    *pointer_to_other_cells = *borders;
    pointer_to_other_cells = pointer_to_other_cells + row_length;
    borders++;
  }
  for (int i = 0; i < row_length; i++)
  {
    *pointer_to_other_cells = *borders;
    pointer_to_other_cells++;
    borders++;
  }
  pointer_to_other_cells--;
  for (int i = 0; i < n_rows - 2; i++)
  {
    pointer_to_other_cells = pointer_to_other_cells - row_length;
    *pointer_to_other_cells = *borders;
    borders++;
  }
}

unsigned char *get_borders_same_cells(unsigned char *image, int xsize, int ysize)
{
  int total_length = (xsize + ysize - 2) * 2;
  unsigned char *fill_borders = calloc(total_length, sizeof(char));
  for (int i = 0; i < xsize + 2; i++)
  {
    // *fill_borders() =
    fill_borders++;
  }
}

void fit_image_cells(unsigned char *playground, unsigned char *image, int xsize, int ysize)
{
  for (int i = 0; i < xsize + 2; i++)
  {
    playground++;
  }
  for (int j = 0; j < ysize; j++)
  {
    for (int i = 0; i < xsize; i++)
    {
      playground++;
      *playground = *image;
      image++;
    }
    playground++;
    playground++;
  }
  for (int i = 0; i < xsize; i++)
  {
    playground++;
  }
  // stampaCells(playground, xsize + 2, ysize + 2);
}

unsigned char *add_upper(unsigned char *borders, unsigned char *playground, int xsize, int ysize)
{
  playground = playground + xsize * (ysize - 1) - 2;
  *borders = *(playground);
  borders++;
  playground = playground - xsize + 3;
  for (int i = 0; i < xsize - 2; i++)
  {
    *borders = *(playground);
    borders++;
    playground++;
  }
  *borders = *(playground - xsize + 2);
  borders++;
  return borders;
}

unsigned char *add_left(unsigned char *borders, unsigned char *playground, int xsize, int ysize)
{
  playground = playground + 2 * xsize - 2;
  for (int i = 0; i < ysize - 2; i++)
  {
    *borders = *(playground);
    playground = playground + xsize;
    borders++;
  }
  return borders;
}

unsigned char *add_down(unsigned char *borders, unsigned char *playground, int xsize, int ysize)
{
  *borders = *(playground + 2 * xsize - 2);
  borders++;
  playground = playground + xsize + 1;
  for (int i = 0; i < xsize - 2; i++)
  {
    *borders = *(playground);
    playground++;
    borders++;
  }
  *borders = *(playground - xsize + 2);
  borders++;
  return borders;
}

void add_right(unsigned char *borders, unsigned char *playground, int xsize, int ysize)
{
  playground = playground + xsize * (ysize - 2) + 1;
  for (int i = 0; i < ysize - 2; i++)
  {
    *borders = *(playground);
    playground = playground - xsize;
    borders++;
  }
}

unsigned char *get_borders(unsigned char *playground, int xsize, int ysize)
{
  unsigned char *borders = calloc((xsize + ysize - 2) * 2, sizeof(char));
  unsigned char *update_pointer;
  update_pointer = add_upper(borders, playground, xsize, ysize);
  update_pointer = add_left(update_pointer, playground, xsize, ysize);
  update_pointer = add_down(update_pointer, playground, xsize, ysize);
  add_right(update_pointer, playground, xsize, ysize);
  return borders;
}

unsigned char *init_playground(unsigned char *playground, unsigned char *image, int xsize, int ysize)
{
  // PER ORA INSERISCO SOLO ELEMENTI DELLA MATRICE STESSA1
  // unsigned char *borders = get_borders_same_cells(image, xsize, ysize);
  fit_image_cells(playground, image, xsize, ysize);
  unsigned char *actual_borders = get_borders(playground, xsize + 2, ysize + 2);
  fill_borders(playground, actual_borders, xsize + 2, ysize + 2);
  free(image);
}

void generate_static(int row_length, int n_rows, unsigned char *base_cells, const unsigned int iterations, const unsigned int dump_step)
{
  unsigned char *pointer_to_secondary_cells = base_cells + row_length * n_rows;

  unsigned char *reading_from_cells;
  unsigned char *writing_to_cells;

  unsigned char *iter_up_pointer;
  unsigned char *iter_med_pointer;
  unsigned char *iter_down_pointer;

  unsigned char *temp_pointer = pointer_to_secondary_cells;
  reading_from_cells = base_cells;

  unsigned char *actual_borders;
  unsigned char sum = 0;
  int dumb_count = 0;
  unsigned char sum_two = 255 * 6;
  unsigned char sum_three = 255 * 5;
  char filename[16];
  for (unsigned int n = 0; n < iterations; n++)
  {
    writing_to_cells = temp_pointer + row_length + 1;
    iter_up_pointer = reading_from_cells;
    iter_med_pointer = iter_up_pointer + row_length;
    iter_down_pointer = iter_med_pointer + row_length;
    dumb_count++;
    for (int row = 0; row < n_rows - 2; row++)
    {
      for (int col = 0; col < row_length - 2; col++)
      {
        sum = *iter_up_pointer + *(iter_up_pointer + 1) + *(iter_up_pointer + 2) +
              *iter_med_pointer + *(iter_med_pointer + 2) +
              *iter_down_pointer + *(iter_down_pointer + 1) + *(iter_down_pointer + 2);

        if (sum == sum_two || sum == sum_three)
        {
          *writing_to_cells = 0;
          //printf("1 ");
        }
        else
        {
          *writing_to_cells = 255;
          //printf("0 ");
        }
        sum = 0;
        writing_to_cells++;
        iter_up_pointer++;
        iter_med_pointer++;
        iter_down_pointer++;
      }
      writing_to_cells = writing_to_cells + 2;
      iter_up_pointer = iter_up_pointer + 2;
      iter_med_pointer = iter_med_pointer + 2;
      iter_down_pointer = iter_down_pointer + 2;
      //printf("\n");
    }
    actual_borders = get_borders(temp_pointer, row_length, n_rows);
    fill_borders(temp_pointer, actual_borders, row_length, n_rows);
    if (dumb_count == dump_step)
    {
      sprintf(filename, "./shots/snapshot_%05d.pgm", n);
      write_pgm_image(temp_pointer, row_length, n_rows, MAX_VAL_PIXEL, filename);
      dumb_count = 0;
    }
    writing_to_cells = reading_from_cells;
    reading_from_cells = temp_pointer;
    temp_pointer = writing_to_cells;
  }
  if (dump_step == 0)
  {
    sprintf(filename, "./shots/snapshot_%05d.pgm", 1);
    write_pgm_image(reading_from_cells, row_length, n_rows, MAX_VAL_PIXEL, filename);
    dumb_count = 0;
  }
}

void generate_ordered(int row_length, int n_rows, unsigned char *base_cells, const unsigned int iterations, const unsigned int dump_step)
{
  unsigned char *iter_up_pointer;
  unsigned char *iter_med_pointer;
  unsigned char *iter_down_pointer;

  unsigned char *actual_borders;
  unsigned char sum = 0;
  int dumb_count = 0;
  unsigned char sum_two = 255 * 6;
  unsigned char sum_three = 255 * 5;
  char filename[16];
  for (unsigned int n = 0; n < iterations; n++)
  {
    iter_up_pointer = base_cells;
    iter_med_pointer = iter_up_pointer + row_length;
    iter_down_pointer = iter_med_pointer + row_length;
    dumb_count++;
    for (int row = 0; row < n_rows - 2; row++)
    {
      for (int col = 0; col < row_length - 2; col++)
      {
        sum = *iter_up_pointer + *(iter_up_pointer + 1) + *(iter_up_pointer + 2) +
              *iter_med_pointer + *(iter_med_pointer + 2) +
              *iter_down_pointer + *(iter_down_pointer + 1) + *(iter_down_pointer + 2);

        if (sum == sum_two || sum == sum_three)
        {
          *(iter_med_pointer + 1) = 0;
          //printf("1 ");
        }
        else
        {
          *(iter_med_pointer + 1) = 255;
          //printf("0 ");
        }
        sum = 0;
        iter_up_pointer++;
        iter_med_pointer++;
        iter_down_pointer++;
      }
      iter_up_pointer = iter_up_pointer + 2;
      iter_med_pointer = iter_med_pointer + 2;
      iter_down_pointer = iter_down_pointer + 2;
      //printf("\n");
    }
    actual_borders = get_borders(base_cells, row_length, n_rows);
    fill_borders(base_cells, actual_borders, row_length, n_rows);
    if (dumb_count == dump_step)
    {
      sprintf(filename, "./shots/snapshot_%05d.pgm", n);
      write_pgm_image(base_cells, row_length, n_rows, MAX_VAL_PIXEL, filename);
      dumb_count = 0;
    }
  }
  if (dump_step == 0)
  {
    sprintf(filename, "./shots/snapshot_%05d.pgm", 1);
    write_pgm_image(base_cells, row_length, n_rows, MAX_VAL_PIXEL, filename);
    dumb_count = 0;
  }
}

int main(int argc, int *argv[])
{

  // int row_length = 6;
  // int n_rows = 6;
  // unsigned char *base_cells = init(row_length, n_rows);

  int xsize = 1000;
  int ysize = 1000;
  int maxval = MAX_VAL_PIXEL;
  int evolution = 0;
  //++++++(long o int?)
  unsigned int iterations = 10000;
  unsigned int dump_step = 1;
  unsigned char *image = read_pgm_image(&xsize, &ysize, &maxval, IMAGE_NAME);
  unsigned char *playground;
  int x_ground_size = xsize + 2;
  int y_ground_size = ysize + 2;
  // stampaCells(image_pointer,xsize,ysize);
  // stampaCells(image, xsize, ysize);

  clock_t inizio, fine;
  double tempo_di_esecuzione;

  switch (evolution)
  {
  case 1:
    inizio = clock();
    playground = calloc(x_ground_size * y_ground_size * 2, sizeof(char));
    // stampaCells(playground, x_ground_size, y_ground_size);
    init_playground(playground, image, xsize, ysize);
    write_pgm_image(playground, x_ground_size, y_ground_size, maxval, "check.pgm");
    generate_static(x_ground_size, y_ground_size, playground, iterations, dump_step);
    fine = clock();

    break;

  case 0:
    inizio = clock();
    playground = calloc(x_ground_size * y_ground_size, sizeof(char));
    init_playground(playground, image, xsize, ysize);
    write_pgm_image(playground, x_ground_size, y_ground_size, maxval, "check.pgm");
    generate_ordered(x_ground_size, y_ground_size, playground, iterations, dump_step);
    fine = clock();
    break;

  default:
    break;
  }
  tempo_di_esecuzione = ((double)(fine - inizio) / CLOCKS_PER_SEC) * 1000;
  printf("Tempo di esecuzione: %f ms\n", tempo_di_esecuzione);
  free(playground);

  // generate(xsize, ysize, cells, iterations, dump_step);

  // write_pgm_image(cells, xsize, ysize, maxval, "imagecopiaincolla.pgm");
}