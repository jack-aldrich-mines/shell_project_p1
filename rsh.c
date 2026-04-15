#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

pid_t pid;
posix_spawnattr_t attr;
int status;

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	
	for (int i = 0; i < sizeof(allowed) / sizeof(allowed[0]); i++) {
		if (strcmp(allowed[i], cmd) == 0)
			return 1;
	}

	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char *argv[64];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

	// seperate line into array of args
	char *token = strtok(line, " \t\n");
	int argc = 0;
	while (token != NULL && argc < 63) {
		argv[argc++] = token;
		token = strtok(NULL, " \t\n");
	}
	argv[argc] = NULL;

	// check if command is allowed
	if (isAllowed(argv[0]) == 0) { // not allowed
		printf("NOT ALLOWED!\n");
		continue;
	}


	// different cmds
	if (strcmp(line,"cd")==0) {
		// not implemtn
		return 0;
	}
	if (strcmp(line,"exit")==0) return 0;
	if (strcmp(line,"help")==0) {
		printf("The allowed commands are:\n\
1: cp\n\
2: touch\n\
3: mkdir\n\
4: ls\n\
5: pwd\n\
6: cat\n\
7: grep\n\
8: chmod\n\
9: diff\n\
10: cd\n\
11: exit\n\
12: help\n");
	}


	// spawn cmds
	posix_spawnattr_init(&attr);
	// spawn
	if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
		perror("spawn failed");
		exit(EXIT_FAILURE);
	}
	// wait to terminate
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid failed");
		exit(EXIT_FAILURE);
	}
	// destroy spawned process
	posix_spawnattr_destroy(&attr);

    }
    return 0;
}
