#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main(){
    key_t key;
    int msgid;

    //ftok generates a unique key
    //"." is a commonly used identifier for the beginning
    //99 is a parameter used to generate a message key, arbitrary number selected by the user
    key = ftok(".", 99);    

    //msgget creates a message queue and returns an identifier
    msgid = msgget(key, 0666 |IPC_CREAT);
    message.mesg_type = 1;

    printf("Enter the Message Data: ");
    fgets(message.mesg_text, MAX, stdin);

    //send message
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("The message sent is : %s \n", message.mesg_text);

    return 0;
}