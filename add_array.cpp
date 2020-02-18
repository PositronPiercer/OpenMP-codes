#include <omp.h>
#include <iostream>
using namespace std;

#define SIZE 10000000
int ar[SIZE];
static int tid;
#pragma omp threadprivate (tid)
int main(){
    int numt, sum = 0;
    double t1, t2;

    for (int i = 0; i < SIZE; i++)
        ar[i] = 1;

    t1 = omp_get_wtime();
    #pragma omp parallel default(none) shared(ar) shared(numt) reduction(+:sum)
    {

        #pragma omp for
        for( int i = 0; i < SIZE; i++){
            sum += ar[i];
        }

    }
    t2 = omp_get_wtime();

    cout<<"Sum : "<<sum<<" time : "<<t2 - t1<<endl;
}