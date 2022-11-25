/* background_demo.c
 * Demonstration of background processes
 * Author: Jason Maloy */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    pid_t child=0;
    int status = 0;

    child = fork();

    if (child == -1) {
	perror("Fork failed, shutting down");
	exit(-1);
    } else if (child) {
	/* We are in the parent */
	long sum = 0;
	char buf[10];
	printf("I am the parent process. We will run until child exits.\nInput numbers one by one and I'll sum them!\n");
	while(!waitpid(-1, &status, WNOHANG)) {
	    memset(buf, '\0', 10);
	    fgets(buf, 10, stdin);
	    buf[strlen(buf)-1] = '\0'; 
	    sum += atoi(buf);
	    printf("Sum is %ld\n", sum);
	}
	printf("Child %d exited with status %d.\n", child, status);
    } else {
	/* We are in the child */
	printf("I am the child process. I'll check in periodically for the next 10 seconds, then exit.\n");
	sleep(2);
	printf("Child checking in!\n");
	sleep(2);
	printf("Are we there yet?\n");
	sleep(2);
	printf("Four seconds left!\n");
	sleep(2);
	printf("Finish your summing!\n");
	sleep(2);
	printf("One sum left! Goodbye!\n");
	exit(0);
    }
}



	
