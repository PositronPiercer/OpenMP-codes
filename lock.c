#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define AR_LENGTH 1000000

typedef struct g{
    int threadId; // the thread which holds the lock currently. -1 if no one holds
    int operatingThreads;
    int currentThread;
}mylock;

int mylock_init(mylock * lck){
    lck->threadId = 0;
    lck->currentThread = 1;
}
int mylock_lock(mylock * lck){
    int myid;

    #pragma omp atomic capture
    {
        lck->threadId++;
        myid = lck->threadId;

    }

    while (myid != lck->currentThread);
}
int mylock_unlock(mylock * lck){

    #pragma omp atomic
    lck->currentThread++;
}
int mylock_destroy(mylock * lck){
}

omp_lock_t l;
int main(){
    omp_init_lock(&l);
    int ar[AR_LENGTH];
    for(int ii = 0; ii < AR_LENGTH; ii++)
        ar[ii] = 1;
    int strideLength = 200;
    int sum = 0;
    
    #pragma omp parallel default(shared)
    {   
        int nthreads = omp_get_num_threads();
        int i = omp_get_thread_num() * strideLength;
        while (i < AR_LENGTH){
            for (int j = 0; j < strideLength && i < AR_LENGTH; j++, i++){
                omp_set_lock(&l);
                sum += ar[i];
                omp_unset_lock(&l);
            }
            i += strideLength * (nthreads - 1); 
        }
        
    }
    omp_destroy_lock(&l);
    printf("Sum [omp_lock]: %d\n", sum);

    mylock mylck;
    mylock_init(&mylck);
    sum = 0;
    #pragma omp parallel default(shared)
    {   
        int nthreads = omp_get_num_threads();
        int i = omp_get_thread_num() * strideLength;
        while (i < AR_LENGTH){
            for (int j = 0; j < strideLength && i < AR_LENGTH; j++, i++){
                mylock_lock(&mylck);
                sum += ar[i];
                mylock_unlock(&mylck);
            }
            i += strideLength * (nthreads - 1); 
        }
        
    }
    mylock_destroy(&mylck);
    printf("Sum [mylock]: %d\n", sum);

}