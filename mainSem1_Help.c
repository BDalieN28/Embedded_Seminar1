
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fork_pid;
    int myID;
    int myParentID;
    int state;

    char input[100];
    char *command;
    
    printf("\n\nSTART\n");
    printf("\n\n");

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        //read command from user
        fgets(input, sizeof(input), stdin);

        //break input into tokens
        command = strtok(input, "\n");

        //If user hits blank enter
        if (command == NULL) {
            continue;
        }

        if(strcmp(command, "help") == 0) {
            printf("\nType \"help\" to display this message.\n");
            printf("\nType \"exit\" to exit the shell.\n");
            printf("\nType anything else to run a child process.\n");
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            printf("\nExiting shell...\n");
            break;
        }
    
    
    fork_pid = fork();
    switch(fork_pid)
    {
        case 0:
            myID = getpid();
            myParentID = getppid();
            printf("\n[ForkPID printed from subprocess/ child =%d; myID = %d; my ParentID = %d] = %d\n", fork_pid, myID, myParentID);
            
            execl(command, command, (char *) NULL);

            //if execl fails
            perror("execl");
            exit(1);
        case -1:
            perror("fork");
            break;
        default:
            myID = getpid();
            myParentID = getppid();
            waitpid(fork_pid,&state, 0);
            printf("\n[ForkPID printed from main process / parent = %d; myID = %d; my ParentID = %d]\n", fork_pid, myID, myParentID);
            
            break;
        
    }

    printf("\n");

} //end while
    
    printf("\n\n");

    return 0;
}