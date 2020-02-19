#include <omp.h>
#include <iostream>
using namespace std;

#define SIZE 1<<20
#define MAX_VAL 2000
int ar[SIZE];
int hist[MAX_VAL];

int main(){
    float t1, t2;
    for (int i = 0; i < SIZE; i++)
        ar[i] = i % MAX_VAL;
    
    for (int i = 0; i < MAX_VAL; i++){
        hist[i] = 0;
    }

    t1 = omp_get_wtime();
    #pragma omp parallel for default(none) shared(ar) shared(hist)
    for (int i = 0; i < SIZE; i++){
        int key = ar[i];

        #pragma omp atomic
        hist[key]++;
    }
    t2 = omp_get_wtime();
    cout<<"Time : "<<t2 - t1<<endl;


}