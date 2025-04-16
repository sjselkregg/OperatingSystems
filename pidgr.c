/**
 *  Program Name:   pidgr.c
 *  Author:         Sam Selkregg
 *  Last Modified:  2/25/2025
 *  Purpose:        Simulation showing process handling (allocation and release).
 *  To Compile:     gcc pidgr.c
 *  To Run (console output):         ./a.out
 *  To Run (text file output):       ./a.out > (textFileName).txt
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>

 #define PID_Min 301
 #define PID_Max 310
 #define MAP_SIZE 10
 #define numIterations 100

 void init_map(int x[]);
 int allocate_pid(int x[]);
 int release_pid(int x[]);
 int scan_map(int x[]);
 void disp_PIDs(int x[]);
 int decide_action();


/**
 * Function:    main
 * Purpose:     runs the business logic of our program
 * Parameters:  N/A
 * Returns:     0 - upon successful completion
 */
 int main(){

    //Initialize inactive and active process pools
    int activePool[MAP_SIZE];
    init_map(activePool);
    srand(time(0));
    
    //Initialize Our Map
    int PIDs[MAP_SIZE];
    init_map(PIDs);
    int action;

    //Loop to run our simulation
    for(int i = 0; i<=numIterations; i++){
        /*
        if(i%2==0){
            usleep(500000 + (rand()%500000));
        }*/
        action = decide_action();
        if(action==1){
            allocate_pid(PIDs);
        }else if(action==2){
            release_pid(PIDs);
        }
        
    }
    

    return 0;
 }

 /**
 * Function:    init_map
 * Purpose:     initializes array elements in our map to 0
 * Parameters:  int x[]: the array we will initialize
 * Returns:     N/A
 */
 void init_map(int x[]){
    for(int i = 0; i<MAP_SIZE; i++){
        x[i] = 0;
    }
 }

 /**
  * Function:   decide_action
  * Purpose:    generates a random number between 1 & 2 to decide to release or allocate a process
  * Parameters: N/A
  * Returns:    1 - allocate process, 2 - release process
  */
 int decide_action(){
    return (rand() % 2) + 1;
 }

 /**
 * Function:    scan_map
 * Purpose:     walks through our array to determine the first available space to allocate a process
 * Parameters:  int x[] - the array we are scanning
 * Returns:     i - the first available space, -1 - signals that our array is full
 */
 int scan_map(int x[]){
    printf("Scanning map ... ");
    for(int i = 0; i < MAP_SIZE; i++){
        if(x[i] == 0){
            return i;
        }
    }
    return -1;
 }


/**
 * Function:    disp_PIDs
 * Purpose:     displays our active PIDs to the user
 * Parameters:  int x[] - the array of PIDs we are displaying
 * Returns:     N/A
 */
void disp_PIDs(int x[]){
    printf(" List: ");
    for(int i = 0; i < MAP_SIZE; i++){
        if(x[i]!=0){
            printf("%d ", x[i]);
        }
    }
    printf("\n");
}

/**
 * Function:        release_pid
 * Purpose:         releases a pid from the active running process map
 * Parameters:      int x[]: the process pool
 * Returns:         the process we release
 *                  -1  no processes are available to be released
 */
int release_pid(int x[]) {
    int avail[MAP_SIZE] = {0};
    int availSpot = 0;
    //determine processes that are running
    for (int i = 0; i < MAP_SIZE; i++) {
        if (x[i] != 0) {
            avail[availSpot++] = x[i]; 
        }
    }
    // If there are no running processes, print an error message
    if (availSpot == 0) {
        printf("There are currently no running processes.\n");
        return -1;
    } else {
        int release = rand() % availSpot; 
        // Announce PID release and release PID in variable
        printf("Released. PID=%d and total PID=%d", avail[release], --availSpot);
        for (int i = 0; i < MAP_SIZE; i++) {
            if (avail[release] == x[i]) {
                x[i] = 0;
                break;  
            }
        }
        disp_PIDs(x);
        //reuse availSpot to return the process we release
        availSpot = release;
    }
    return avail[availSpot]; 
}


/**
 * Function:        allocate_pid
 * Purpose:         allocates processes to our map 
 * Parameters:      int x[]: the array we are allocating processes to
 * Returns:         -1 if we are unable to allocate a process
 *                  the PID of the process we allocated if allocated successfully
 */
 int allocate_pid(int x[]){
    int maxAllocated = 0, maxAllocatedValue = 301;
    int totalPID= 0;
    //determine which process should be allocated
    for(int i = 0; i < MAP_SIZE; i++){
        if(x[i]>=maxAllocatedValue && maxAllocatedValue<=PID_Max){
            maxAllocatedValue = x[i] + 1;
            maxAllocated = i + 1;
        }
        if(x[i]!=0){
            totalPID += 1;
        }
    }
    if(maxAllocated == 0){ //allocate our first process
        x[0] = PID_Min;
        printf("Allocated PID=%d and Total PID=%d", PID_Min, totalPID + 1);
        disp_PIDs(x);
        return PID_Min;
    }else if(maxAllocatedValue == PID_Max + 1){ //if the new maxAllocatedValue > PID_MAX, scan map for other free spaces
        int freeSpot;
        freeSpot = scan_map(x);
        if(freeSpot == -1){
            printf("There are no available processes to be allocated.");
            disp_PIDs(x);
            return -1;
        }
        x[freeSpot] = x[freeSpot - 1] + 1;
        if(x[freeSpot]<PID_Min){    //handle error where we were allocating below the PID_Min
            x[freeSpot]+=(PID_Min-1);
        }
        printf("Allocated PID=%d and Total PID=%d", x[freeSpot], totalPID + 1);
        disp_PIDs(x);
        return x[freeSpot];
    }else{      //normal allocation
        x[maxAllocated] = PID_Min + maxAllocated;
        printf("Allocated PID=%d and Total PID=%d", x[maxAllocated], totalPID + 1);
        disp_PIDs(x);
        return x[maxAllocated];
    }
    printf("Method Fail\n");
    return -1;
 }

