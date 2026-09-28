
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int fork_pid;
	char input[100];
	int myID;
	int myParentID;

	printf("\n\nSTART\n");
	printf("\n\n");

	while (1) {
		printf("myshell> ");
		fflush(stdout);

		//read command from user
		if (fgets(input, sizeof(input), stdin) == NULL) {
			printf("\nEnd of input. Exiting shell.\n");
			break;
		}

		//parsing input based on tab, newline, and space. Placing then into an array

		char *arguments[10]; //Holds up to 10 words
		int count = 0;

		char *token = strtok(input, " \t\n"); //Using temp var token so that we do not need to instatiate 'arguments' in while loop

		while (token != NULL && count < 9) { // Note: count < 9 as 10th element reserved for null
			arguments[count] = token;
			count++;
			token = strtok(NULL, " \t\n"); //Using NULL instead of input to continue down var input

		}
		arguments[count] = NULL; //adds null at final count


		//If user hits blank enter
		if (count == 0) { //Checking that the only element is Null or that it's blank
			continue; // If so, reloop back to the start of while loop
		}

		//help custom command
		if(strcmp(arguments[0], "help") == 0) {
			printf("\nType \"help\" to display this message.\n");
			printf("\nType \"exit\" to exit the shell.\n");
			printf("\nType ls /bin to list the /bin directory.\n");
			printf("\nType anything else to run a child process.\n");
			continue;
		}

		//exit custom command
		if (strcmp(arguments[0], "exit") == 0) {
			printf("\nExiting shell...\n");
			break;
		}

		//fork + ls command implementation
		fork_pid = fork(); // creating a second process
		if (fork_pid < 0)
		{
			perror("fork error");
		} else if (fork_pid == 0) { // if process is child process
			myID = getpid();
			myParentID = getppid();
			printf("\n[ForkPID printed from subprocess/ child =%d; myID = %d; my ParentID = %d] = %d\n", fork_pid, myID, myParentID);

			execvp(arguments[0], arguments); // run ls command on everything in ls

			perror("execvp failed"); //runs line 72 if fails since a fail execvp continues else if
			_exit(EXIT_FAILURE);// terminates child

		} else { // if process is parent process
			int state;
			int pid_status = waitpid(fork_pid,&state,0); //which process id to wait for, variable address passed into state, behavior flag
			
			myID = getpid();
			myParentID = getppid();
			printf("\n[ForkPID printed from main process / parent = %d; myID = %d; my ParentID = %d]\n", fork_pid, myID, myParentID);

			if (pid_status == -1) {
				perror("pid failed");
			}


		}


		printf("\n");

	} //end while

	printf("\n\n");

	return 0;
}