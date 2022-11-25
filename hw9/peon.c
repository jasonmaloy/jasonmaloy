/* peon.c
 * Peon Shell - A unix shell coded in C
 * Important notes:
 *	pgid 1 - background processes
 *	pgid 2 - "foreground" processes (will hang until return value is reaped)
 * Author: Jason Maloy */

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t parse_command(char *file, char *argv[], int in, int out);
void sig_handler(int sig);

pid_t child; // Fork variable; kept up here for handler access

pid_t parse_command(char *file, char *argv[], int in, int out) {
    dup2( in, STDIN_FILENO );
    dup2( out, STDOUT_FILENO );
    if (-1 == execvp(file, argv)) {
	perror("Exec failed\n");
	exit(-1);
    }
    return getpid();
}

void sig_handler(int sig) {
    if (sig==SIGINT) {
	kill(child, SIGINT);
    }
}


int main() {
    /* File pointers for redirection. Default to stdin and stdout. */
    int read = STDIN_FILENO;
    int write = STDOUT_FILENO;
    char input[1025];
    char *argv[100];
    printf("What you want?\n");
    while(1) {
	read = STDIN_FILENO;
	write = STDOUT_FILENO;
	memset(argv, '\0', sizeof(argv));

	/*Ignore ctrl+c*/
	signal(SIGINT, SIG_IGN);

	/* Prompt user for input */
	int status;
	wait(&status);
	char * dirname = get_current_dir_name();	
	printf("%s ", dirname);
	free(dirname);
	fgets(input, 1025, stdin);
	if (input[strlen(input)-1] == '\n') { /* replace newline w/ null byte*/
	    input[strlen(input)-1] = '\0';
	}

	/*make into vector*/
	if (input[0] == '\0') {
	    /*Restart shell loop on a null input*/
	    continue;
	} else {
	    char *s = NULL;
	    int curr_index = 1;
	    argv[0] = strtok(input, " "); /* First item added manually */
	    /* Redirection vars
	     * When we reach a < or > token, don't copy it over;
	     * instead, note that we have a redirect and set it accordingly */
	    int next_is_input = 0;
	    int next_is_output = 0;
	    int abort = 0; /* If something goes wrong, abort the command */
	    
	    /* Tokenize the command line input */
	    while ( (s=strtok(NULL, " ")) != NULL) {
		if (next_is_input) {
		    if (-1 == (read=open(s, O_RDONLY))) {
			abort = 1;
		    }
		    next_is_input = 0;
		    continue;
		} else if (next_is_output) {
		    chmod(s, S_IRUSR|S_IROTH|S_IWUSR);
		    write=creat(s, O_WRONLY);
		    chmod(s, S_IRUSR|S_IROTH|S_IWUSR);
		    next_is_output = 0;
		    continue;
		} else {
		    if (strcmp(s, "<") == 0) {
			next_is_input = 1;
			continue;
		    } else if (strcmp(s, ">") == 0) {
			next_is_output = 1;
			continue;
		    }
		    argv[curr_index] = s;
		    curr_index++;
		}
	    }
	    

	    /* Check to see if & is the last token. */
	    if (strcmp(argv[curr_index-1], "&") == 0) {
		perror("I saw a valid & token.\nGarrosh Shell doesn't currently work with background processes; see the README for more info.\nYour process will be run in the foreground.\n");
		argv[curr_index-1] = NULL;
	    }
	    if (abort) {
		perror("Invalid file; aborting command.");
		abort = 0;
		break;
	}
	
	/* Built-in commands:
	 * exit: Terminate shell input loop
	 * myinfo: Print out PID and PPID in readable format
	 * cd: Change working directory to $HOME
	 * cd <dir>: change working directory to dir */
	if (strcmp(argv[0], "exit") == 0) {
	    printf("Me not that kind of orc!\n");
	    break;
	} else if (strcmp(argv[0], "myinfo") == 0) {
	    printf("Process ID: %d\nParent PID: %d\n", getpid(), getppid());
	    continue;
	} else if (strcmp(argv[0], "cd") == 0) {
	    if (argv[1] == '\0') {
		chdir(getenv("HOME"));
	    } else {
		chdir(argv[1]);
	    }
	    continue;
	}

	/* fork and exec */

	child = fork();

	if (child) {
	    /* parent */
	    signal(SIGINT, sig_handler);
	} else { 
	   /* child */
	    parse_command(argv[0], argv, read, write);
			
	    exit(-1);
	}
    }
}
}
