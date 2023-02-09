#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 32
#define COLS 32
#define ITERATIONS 100

unsigned int grid[ROWS];
unsigned int temp[ROWS];

unsigned int get_cell(unsigned int row, unsigned int col) {
    return (grid[row] >> col) & 1;
}

void set_cell(unsigned int row, unsigned int col, unsigned int value) {
    if (value) {
        grid[row] |= 1 << col;
    } else {
        grid[row] &= ~(1 << col);
    }
}

unsigned int count_neighbors(unsigned int row, unsigned int col) {
    unsigned int count = 0;
    count += get_cell(row - 1, col - 1);
    count += get_cell(row - 1, col);
    count += get_cell(row - 1, col + 1);
    count += get_cell(row, col - 1);
    count += get_cell(row, col + 1);
    count += get_cell(row + 1, col - 1);
    count += get_cell(row + 1, col);
    count += get_cell(row + 1, col + 1);
    return count;
}

void update_grid(void) {
    for (unsigned int row = 0; row < ROWS; row++) {
        for (unsigned int col = 0; col < COLS; col++) {
            unsigned int neighbors = count_neighbors(row, col);
            if (get_cell(row, col)) {
                if (neighbors < 2 || neighbors > 3) {
                    set_cell(row, col, 0);
                }
            } else {
                if (neighbors == 3) {
                    set_cell(row, col, 1);
                }
            }
        }
    }
    for (unsigned int row = 0; row < ROWS; row++) {
        grid[row] = temp[row];
    }
}

void print_grid(void) {
    for (unsigned int row = 0; row < ROWS; row++) {
        for (unsigned int col = 0; col < COLS; col++) {
            printf("%c", get_cell(row, col) ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    srand(time(NULL));
    for (unsigned int row = 0; row < ROWS; row++) {
        for (unsigned int col = 0; col < COLS; col++) {
            set_cell(row, col, rand() % 2);
        }
    }
    for (unsigned int i = 0; i < ITERATIONS; i++) {
        update_grid();
        print_grid();
    }
    return 0;
}
