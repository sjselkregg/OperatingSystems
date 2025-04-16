/**
 *  Program Name:   copy.c
 *  Author:         Sam Selkregg
 *  Last Modified:  2/11/2025
 *  Purpose:        Copies the contents of a file to another file using system calls.
 *  To Compile:     gcc copy.c
 *  To Run:         ./a.out
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define STD_SIZE 1024

/**
 *  Function:   main
 *  Purpose:    sequentially runs our program step-by-step to copy a file
 *  Parameters: N/A
 *  Returns:    integer 0 - upon successful completion
 */
int main(){

    //Display prompt on screen
    char prompt[] = "Enter the name of a file you want to copy!\n";
    write(1, prompt, sizeof(prompt)-1);

    //read the user input
    char buffer[STD_SIZE];
    ssize_t charsRead = read(0, buffer, sizeof(buffer)-1);
    if(charsRead>0){
        buffer[charsRead-1] = '\0';
    }

    //Display second prompt
    char prompt2[] = "Enter the name for the new file to copy contents into!\n";
    write(1, prompt2, sizeof(prompt2)-1);

    //read the second user input
    char buffer2[STD_SIZE];
    ssize_t charsRead2 = read(0, buffer2, sizeof(buffer2)-1);
    if(charsRead2>0){
        buffer2[charsRead2-1] = '\0';
    }

    //open a file for reading, if the file doesnt exist, ITS ERROR TIME
    int stinky = open(buffer, O_RDONLY);
    if(stinky==-1){
        perror("ERROR OPENING THE FILE, YOU HAVE BEEN PENALIZED, 500 PUSH UPS, NOW!!!!!!");
        exit(0);
    }

    //open a new file specified by the user, to write to, also check if it already exists
    int smelly = open(buffer2, O_WRONLY|O_CREAT|O_EXCL, 0644);
    if(smelly == -1){
        if(errno==EEXIST){
            perror("ARE YOU KIDDING ME!?! YOU GAVE A FILE THAT ALREADY EXISTS, YOU SHOULD KNOW BETTER!\n");
            close(stinky);
            exit(0);
        }else{
            perror("ERROR CREATING THE FILE, PROBABLY NOT YOUR FAULT, STILL, DROP AND GIVE ME 2O!!!!\n");
        }
        close(stinky);
        exit(0);
    }


    //Copy the file using a loop, read 1kb at a time until all of the file is copied into our new file
    char theBuffer[STD_SIZE];
    ssize_t stuffRead;
    while((stuffRead = read(stinky, theBuffer, STD_SIZE))>0){
        if((write(smelly, theBuffer, stuffRead))!= stuffRead){  //if write does not return the size we expected to read, there is an issue
            perror("THERE HAS BEEN A PROBLEM COPYING THE CONTENTS, TRY AGAIN");
            close(stinky);
            close(smelly);
            exit(0);
        }
    }

    //if reading returns -1, there has been an error reading from the source file, so we should let the user know and abort
    if(stuffRead == -1){
        perror("WE CAN'T EVEN READ FROM THE SOURCE FILE, MAYBE YOU SHOULD DROP OUT AND WORK AT MCDONALDS");
        close(stinky);
        close(smelly);
        exit(0);
    }

    //since we made it this far, we can assume success, and let our user know
    char successPrompt[] = "CONGRATS YOU DID IT!!! YOU ARE NOW THE WORLD HEAVYWEIGHT CHAMPION, GO ENJOY A BEER!\n";
    write(1, successPrompt, sizeof(successPrompt) - 1);
    //close the files
    close(stinky);
    close(smelly);

    return 0;

}