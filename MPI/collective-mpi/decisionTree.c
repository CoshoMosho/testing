#include <stdio.h>

float buildTreeModel(float parameter[]);
int comparing(float valori[], float limite, int verso);
int main(int argc, char *argv[]){

    float val1[3] = {1, 2, 1};
    float val2[3] = {3, 1, 0};
    float val3[3] = {4, 2, 1};
    //float arr[][] = {val1,val2,val3};
    float arr[] = {1,2,3,4,5,6,7,8};
    int t = comparing(arr,3.0,1);
    printf("iao");
    printf("%d",t);
}

float buildTreeModel(float parameter[]){

}

int comparing(float valori[], float limite, int verso){
    //occhio al verso come integer
    int errori = 0;
    for (int i = 0; i < sizeof(valori); i++)
    {
        if (valori[i] < verso * limite)
        {
            errori++;
        } 
    } 
    return errori;
}