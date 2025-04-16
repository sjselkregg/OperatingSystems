#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    //fork a child process
    pid = fork();

    printf("fork() returns pid = %d\n", pid);
    //0 is the pid of the child that you see in the parent process
    if(pid<0) {         //error occured
        fprintf(stderr, "Fork Failed");
        return 1;
        //execl allows you to specify code path
    } else if(pid == 0){    //child process here
        printf("PID of the parent process is %d \n", getppid());    //getpid() gets pid of process being executed
        execlp("/bin/ls", "ls", NULL);  //execlp (program path to be run, program to be run, parameters)
                                        //can add as many arguments as command requires, terminate w NULL
        //once you execute above command this will override any code here
    }else {     //parent process
        // could say pid = wait(NULL);
        pid = wait(NULL);     //wait for child to complete
        printf("Child Complete\n");
        printf("PID RETURNED: %d\n", pid);
    }

    return 0;
}