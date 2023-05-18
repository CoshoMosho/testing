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

    // int u  = 2412412414;
    // int *up = u;
    // int uu = *up;


    int bbb = 3;
    int ccc = 2;
    int d = 3%2;
    int e = 2%3;


    int A[] = {1,2,3};
    printf("%d \n", A);
    printf("%d \n", &A[0]);
    printf("%d \n", A[0]);
    printf("%d \n", *A);

    testARrray(A,3);
    for (int i = 0; i < 3; i++)
    {
        printf("%d\n",A[i]);
    }

    printf("end");
}

testARrray(int* A, int size){
    for (int i = 0; i < size; i++)
    {
        A[i] = A[i]*2;
    }
    
}