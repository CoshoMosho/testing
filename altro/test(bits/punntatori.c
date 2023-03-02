#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("res");
    int a = 2;
    int *p;
    long *longp;
    p = &a;
    longp = &a;
    long c =  &a;
    int z = sizeof(a);
    int zz = sizeof(p);
    int zzz = sizeof(c);

    int u  = 2412412414;
    int *up = u;
    int uu = *up;

    printf("end");
}