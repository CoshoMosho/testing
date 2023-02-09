#include <stdio.h>

int arr1[] = {1,1,2};
int arr2[] = {1,11,21};
int arr3[] = {2,10,2};

double tornaPuntoMedio();
int ritornaTrueSeValoreMaggioreTreshold();

int main(int argc, char **argv ) {
  // int punto1 = 1;
  // printf("\n %d",arr1);
  // printf("\n %d",&arr1);
  // printf("\n %d",*arr1);
  double test = tornaPuntoMedio(3.2,5.2);
  double test1 = tornaPuntoMedio(-3.2,5.2);
  double test2 = tornaPuntoMedio(-3.2,-5.2);
  double test3 = tornaPuntoMedio(3.2,-5.2);
  printf("\n %f",test);
  printf("\n %f",test1);
  printf("\n %f",test2);
  printf("\n %f",test3);
  printf("test\n");
  return 0;
}

int confrontaValoreConTreshold(double treshold, double valore){
  if(valore > treshold){
    return 1;
  }else if(valore < treshold){
    return -1;
  }else{
    return 0;
  }
}

double tornaPuntoMedio(double a, double b){
  return (a+b)/2;
}



