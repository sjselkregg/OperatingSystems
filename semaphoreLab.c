#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ITER 10000

int cnt = 0;
sem_t mutex;

void *Count(void *tid){
    
    int i;
    sem_wait(&mutex);
    for(i=0; i < ITER; i++){
        
        cnt++;
        
    }
    sem_post(&mutex);
    
}

int main(){

    pthread_t tid1, tid2;
    
    sem_init(&mutex, 0, 1);
    
    pthread_create(&tid1, NULL, Count, (void *)1);
    
    pthread_create(&tid2, NULL, Count, (void *)2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if(cnt < 2 * ITER){
        printf("\n Race Struck *** count is %d, should be %d\n", cnt , 2*ITER);
    }else{
        printf("\n Race Avoided. Count is [%d]\n", cnt);
    }
    sem_destroy(&mutex);
    pthread_exit(NULL);


    return 0;
}