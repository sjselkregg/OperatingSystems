#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ITER 10000

int cnt = 0;
pthread_mutex_t mutex;
void *Count(void *tid){
    
    int i;
    pthread_mutex_lock(&mutex);
    for(i=0; i < ITER; i++){
        
        cnt++;
        
    }
    pthread_mutex_unlock(&mutex);
}

int main(){

    pthread_t tid1, tid2;
    

    
    pthread_create(&tid1, NULL, Count, (void *)1);
    
    pthread_create(&tid2, NULL, Count, (void *)2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if(cnt < 2 * ITER){
        printf("\n Race Struck *** count is %d, should be %d\n", cnt , 2*ITER);
    }else{
        printf("\n Race Avoided. Count is [%d]\n", cnt);
    }

    pthread_exit(NULL);


    return 0;
}