#include <stdio.h>

void printBits(unsigned char x) {
   int i;
   for(i=7; i>=0; i--)
       printf("%d", (x>>i)&1);
   printf("\n");
}

void altro_test()
{
    int tentativi = 2;

    printf("");
}

void take_snapshot(int iteration_number) {
  char filename[16];
  sprintf(filename, "snapshot_%05d.pgm", iteration_number); // creates the string "snapshot_nnnnn.pgm" with 5 digits padded with zeros
  // take snapshot and save it as the filename string
  printf(filename);
  printf("\n");
}


int main() {
//    unsigned char c = 255;
//    unsigned char zero = 0;
//    unsigned char sum_two = 255*2;
//    printBits(c);
//    printBits(c+c);
//    printBits(c+c+c);
//    printBits(c+c+c+c);
//    printBits(c+c+c+c+c);
//    printBits(c+c+c+c+c+c);
//    printBits(c+c+c+c+c+c+c);
//    printBits(c+c+c+c+c+c+c+c);
//    printBits(sum_two);
//    printBits(zero+c+c);
//    altro_test();
    for (int i = 0; i < 99999; i += 351)
    {
        take_snapshot(i);
    }
    


   return 0;
}
