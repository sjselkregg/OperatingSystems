/**
 *  Program Name:   pidmgrT.c
 *  Author:         Sam Selkregg
 *  Last Modified:  4/9/2025
 *  Purpose:        Multithreaded simulation showing process handling (allocation and release). 
 *  To Compile:     gcc pidmgrT.c
 *  To Run (console output):         ./a.out
 *  To Run (text file output):       ./a.out > (textFileName).txt
 */

 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <unistd.h>

 #define PID_Min 300
 #define PID_Max 304
 #define numThreads 10
 #define numIterations 10

 //#define PID_Min 300
 //#define PID_Max 350
 //#define numThreads 40
 //#define numIterations 10
 
 #define mapSize (PID_Max - PID_Min + 1)
 int pids[mapSize];
 pthread_mutex_t mutex;


 void init_map();
 void *allocator(void *param);
 int allocate_pid();
 int scan_map();
 int release_pid(int x);


  /**
 * Function:    main
 * Purpose:     the runner of our program, creates the threads to run their processes, then joins all threads to finish operation
 * Parameters:  N/A
 * Returns:     0 upon successful exit
 */
 int main() {

    init_map(pids);
    
    pthread_t threads[numThreads];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    // Create the threads
    for (int i = 0; i < numThreads; i++) {
        int *arg = malloc(sizeof(int));  // Allocate memory for thread argument
        *arg = i;
        pthread_create(&threads[i], &attr, allocator, arg);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
 }


  /**
 * Function:    init_map
 * Purpose:     initializes array elements in our map to 0
 * Parameters:  N/A
 * Returns:     N/A
 */
void init_map(){
    printf("\nInitializing Map...\n");
    printf("PID Range: %d - %d\n", PID_Min, PID_Max);
    printf("Number of Threads: %d\n", numThreads);
    printf("Number of Iterations: %d\n", numIterations);
    for(int i = 0; i<mapSize; i++){
        pids[i] = 0;
    }
 }

  /**
 * Function:    *allocator
 * Purpose:     our thread function, will allocate a pid, sleep, then release the same pid and output messages accordingly
 * Parameters:  *param: parameter used to extract the thread index
 * Returns:     N/A
 */
 void *allocator(void *param){
    
    int index = *((int *)param);  // Extract thread index
    free(param);  // Free allocated memory

    for(int i = 0; i < numIterations; i++){

        //sleep for 1-2 seconds
        int sleep1 = (rand() % 2) + 1;
        sleep(sleep1);

        pthread_mutex_lock(&mutex);
        int allocated = allocate_pid();
        if(allocated!=-1){
            printf("Thread %d - Allocation Request ... Allocated %d\n", index, (allocated+PID_Min));
        }else{
            printf("Thread %d - Allocation Request ... No PID Available\n", index);
        }
        pthread_mutex_unlock(&mutex);
        
        //sleep for 1-3 seconds
        int sleep2 = (rand() % 3) + 1;
        sleep(sleep2);

        //release the pid
        pthread_mutex_lock(&mutex);
        int released = release_pid(allocated);
        if(released==-1){
            printf("Thread %d has no PID to release.\n", index);
        }else{
            printf("Thread %d: Released PID %d\n", index, (released+PID_Min));
        }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(0);

}

 /**
 * Function:    allocate_pid
 * Purpose:     allocates a pid within the pid map
 * Parameters:  N/A
 * Returns:     the location in the map of the pid we allocated, -1 if failed
 */
int allocate_pid() {
    // If highest PID is already allocated, scan the map
    if (pids[mapSize - 1] == 1) {
        int i = scan_map();
        if (i != -1) {
            pids[i] = 1;
        }
        return i; // Either the PID allocated or -1 if none found
    }

    int allocate = -1;
    int maxAllocated = 0, totalAllocated=0;
    //if max pid is not allocated, allocated the highest available pid in order
    for (int i = 0; i < mapSize; i++) {
        if(pids[i]==1){
            maxAllocated=i;
            totalAllocated++;
        }
    }

    if(totalAllocated==0){
        pids[0] = 1;
        return 0;
    }else{
        pids[++maxAllocated] = 1;
        return maxAllocated;
    }

    // If we somehow get here, all PIDs are in use, with current requirements should never occur
    return -1;
}

 /**
 * Function:    scan_map
 * Purpose:     scans the map to determine a position of a pid we can allocate in the map
 * Parameters:  N/A
 * Returns:     the position of an available space in the map for a pid to be allocated
 */
 int scan_map(){
    printf("Scanning map ... ");
    for(int i = 0; i < mapSize; i++){
        if(pids[i]==0){
            return i;
        }
    }
    return -1;
 }


 /**
 * Function:    release_pid
 * Purpose:     initializes array elements in our map to 0
 * Parameters:  int x: the space of the pid to be released
 * Returns:     the pid we release, -1 if failed
 */
 int release_pid(int x){
    if (x == -1 || pids[x] == 0) {
        return -1;
    } else {
        pids[x] = 0; 
        return x;
    }
}