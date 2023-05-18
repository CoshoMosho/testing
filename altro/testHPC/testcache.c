#include <stdio.h>
#include <time.h>

#define KB 1024
#define MB 1024 * 1024

int main(){
   
   long start, stop;
   int lengthMod = 8*KB -1;
   double totalTime, timeTaken;
   int tmp;
   double avg;

   static int array[8*KB];
   int i,j;
   unsigned int k;
/*Change the step of array iteration and observe the change in time taken for computation.
The step('i' value) at which there is a significant change in time taken will correspond to the cache line size*/
   for(i=1;i<=2048;i*=2){
      totalTime=0;
      for(j=0;j<6;j++){
         start = clock();
         void *add = &start;
         for(k=0;k<512*KB;k++){
            tmp +=array[(k*i)&lengthMod];   
         }
         timeTaken = (double)(clock() - start)/CLOCKS_PER_SEC;
         totalTime += timeTaken;
      }
      totalTime /= 6;
      printf("For i value: %d    Time Taken:%lf\n",i,totalTime);
   } 
   
}