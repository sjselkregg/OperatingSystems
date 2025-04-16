#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10
#define NUMBERS_COUNT 10000

float numbers[NUMBERS_COUNT];  // Global array for accessibility
float globalSum = 0;  // Shared sum variable
pthread_mutex_t mutex;  // Mutex for synchronization

void *runner(void *param); // Function for threads

int main() {
    // Initialize numbers array
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        numbers[i] = i + 1;  // Filling with values 1 to 10000
    }

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Get default attributes
    pthread_attr_init(&attr);

    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));  // Allocate memory for thread argument
        *arg = i;
        pthread_create(&threads[i], &attr, runner, arg);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    printf("sum = %.2f\n", globalSum);
    return 0;
}

// The thread will begin execution here
void *runner(void *param) {
    int index = *((int *)param);  // Extract thread index
    free(param);  // Free allocated memory

    int start = index * (NUMBERS_COUNT / NUM_THREADS);
    int end = start + (NUMBERS_COUNT / NUM_THREADS);

    float localSum = 0;
    for (int i = start; i < end; i++) {
        localSum += numbers[i];
    }

    // Update global sum safely
    pthread_mutex_lock(&mutex);
    globalSum += localSum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}
