#include <stdio.h>

int main()
{
    printf("gg");

    unsigned char* cells;
	unsigned char* temp_cells;
	unsigned int width;
	unsigned int height;
	unsigned int length_in_bytes;

    unsigned int x, y, count;
	unsigned int h = height, w = width;
	unsigned char *cell_ptr;

	// Copy to temp map to keep an unaltered version
	memcpy(temp_cells, cells, length_in_bytes);

	// Process all cells in the current cell map
	cell_ptr = temp_cells;
	for (y = 0; y < h; y++) {

		x = 0;
		do {

			// Zero bytes are off and have no neighbours so skip them...
			while (*cell_ptr == 0) {
				cell_ptr++; // Advance to the next cell
				// If all cells in row are off with no neighbours go to next row
				if (++x >= w) goto RowDone;
			}

			// Remaining cells are either on or have neighbours
			count = *cell_ptr >> 1; // # of neighboring on-cells
			if (*cell_ptr & 0x01) {

				// On cell must turn off if not 2 or 3 neighbours
				if ((count != 2) && (count != 3)) {
					ClearCell(x, y);
					DrawCell(x, y, OFF_COLOUR);
				}
			}
			else {

				// Off cell must turn on if 3 neighbours
				if (count == 3) {
					SetCell(x, y);
					DrawCell(x, y, ON_COLOUR);
				}
			}

			// Advance to the next cell byte
			cell_ptr++;

		} while (++x < w);
	RowDone:;
	}
}