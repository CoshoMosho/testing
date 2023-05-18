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
unsigned int number_of_generations = 10;

//-altre impostazioni

// matrice game of life
// unsigned char *cells;
// unsigned char *temp_cells;
// unsigned int width;
// unsigned int height;

void stampaCells(char *cells, int row_length, int n_rows)
{
  for (int j = 0; j < n_rows; j++)
  {
    int multRiga = j * row_length;
    for (int i = 0; i < row_length; i++)
    {
      if (cells[multRiga + i] == 1)
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
}

char *init(int dimension)
{
  unsigned char *cells = calloc(dimension, sizeof(char));
  // cells[0] = 1;
  // cells[4] = 1;
  // cells[5] = 1;
  // cells[3] = 1;
  // cells[10] = 1;
  return cells;
}

void static_evolution()
{
}
void ordered_evolution()
{
}

//
char *random_borders_generator(int length, int width)
{
  int dimension = 2 * (length + width - 4);
  unsigned char *random_values = calloc(dimension, sizeof(char)); // array che conterrà i valori casuali generati

  srand(time(NULL)); // inizializza il generatore di numeri casuali

  for (int i = 0; i < dimension; i++)
  {
    // genera un numero casuale compreso tra 0 e 1
    int random_number = rand() % 2;

    // converte il numero casuale in un char
    random_values[i] = (char)random_number;
  }

  // stampa i valori casuali generati
  for (int i = 0; i < dimension; i++)
  {
    if (i % 4 == 0)
    {
      printf(" - ");
    }
    
    printf("%d ", random_values[i]);
  }
  printf("\n");
  return random_values;
}

void fill_borders(unsigned char *pointer_to_other_cells, int row_length, int n_rows)
{
  stampaCells(pointer_to_other_cells, row_length, n_rows);

  unsigned char *random_values = random_borders_generator(row_length, n_rows);
  unsigned char *fill_leftup_up_pointer = pointer_to_other_cells;

  // RIPARTIRE DA QUA FACEVO ANGOLI
  *fill_leftup_up_pointer = *(random_values + row_length - 3);
  printf("\n");
  printf("il valore random %d ", *(random_values + row_length - 3));
  printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
  fill_leftup_up_pointer++;
  for (int f = 0; f < row_length - 2; f++) // up left to up right
  {
    *fill_leftup_up_pointer = *random_values;
    printf("\n");
    printf("il valore random %d ", *random_values);
    printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
    fill_leftup_up_pointer++;
    random_values++;
  }
  *fill_leftup_up_pointer = *(random_values - row_length + 2);
  
  printf("\n");
  printf("il valore random %d ", *(random_values - row_length + 2));
  printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
  fill_leftup_up_pointer++;
  for (int f = 0; f < n_rows - 2; f++) // up left 2nd row to down left
  {
    *fill_leftup_up_pointer = *random_values;
    printf("\n");
    printf("il valore random %d ", *random_values);
    printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
    fill_leftup_up_pointer = fill_leftup_up_pointer + row_length;
    random_values++;
  }
  *fill_leftup_up_pointer = *(random_values - n_rows + 2);
  
  printf("\n");
  printf("il valore random %d ", *(random_values - n_rows + 2));
  printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
  fill_leftup_up_pointer++;
  for (int f = 0; f < row_length - 2; f++) // down left to down right
  {
    *fill_leftup_up_pointer = *random_values;
    printf("\n");
    printf("il valore random %d ", *random_values);
    printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
    fill_leftup_up_pointer++;
    random_values++;
  }
   *fill_leftup_up_pointer = *(random_values - row_length + 2);
  
  printf("\n");
  printf("il valore random %d ", *(random_values - row_length + 2));
  printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
  fill_leftup_up_pointer = fill_leftup_up_pointer - row_length;
  for (int f = 0; f < n_rows - 2; f++) // down right to up right
  {
    *fill_leftup_up_pointer = *random_values;
    printf("\n");
    printf("il valore random %d ", *random_values);
    printf("è stato assegnato all'indirizzo %p ", fill_leftup_up_pointer);
    fill_leftup_up_pointer = fill_leftup_up_pointer - row_length;
    random_values++;
  }

  printf("\n");
  stampaCells(pointer_to_other_cells, row_length, n_rows);
}

void next_generation()
{
  int row_length = 6;
  int n_rows = 6;
  unsigned char *base_cells = init(row_length * n_rows * 2);
  unsigned char *temp_cells = base_cells + row_length * n_rows + row_length + 1;
  unsigned char *pointer_to_other_cells = base_cells + row_length * n_rows;
  // stampaCells(base_cells, row_length, n_rows);
  // printf("\n");
  // stampaCells(temp_cells, row_length, n_rows);

  fill_borders(base_cells, row_length, n_rows);
  // memcpy(temp_cells, cells, sizeof(temp_cells));
  unsigned char *iter_up_pointer;
  unsigned char *iter_med_pointer;
  unsigned char *iter_down_pointer;
  // bool to_temp_cells = false;
  // rivedere qua sotto
  int iteration_left = row_length * (n_rows - 2) - 2;
  int counter = iteration_left;
  int sum = 0;
  for (int n = 0; n < number_of_generations; n++)
  {
    iter_up_pointer = base_cells;
    iter_med_pointer = iter_up_pointer + row_length;
    iter_down_pointer = iter_med_pointer + row_length;
    counter = iteration_left;
    sum = 0;
    while (counter != 0)
    {
      sum = *iter_up_pointer + *(iter_up_pointer + 1) + *(iter_up_pointer + 2) +
            *iter_med_pointer + *(iter_med_pointer + 2) +
            *iter_down_pointer + *(iter_down_pointer + 1) + *(iter_down_pointer + 2);

      // controllare ||
      if (sum == 2 || sum == 3)
      {
        *temp_cells = 1;
      }
      else
      {
        *temp_cells = 0;
      }
      temp_cells++;
      iter_up_pointer++;
      iter_med_pointer++;
      iter_down_pointer++;
      counter--;
    }

    printf("\n");
    stampaCells(pointer_to_other_cells, row_length, n_rows);
    iter_up_pointer = pointer_to_other_cells;
  }

  // modifiche ai passi successivi

  bool quitWhile = false;

  printf("mah %d", quitWhile);

  // stampa configurazione
}

int main(int argc, int *argv[])
{
  next_generation();
}
