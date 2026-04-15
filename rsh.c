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
	if (strcmp(argv[0], "cd")==0) {
		if (argc == 1) {
			printf("-rsh: cd: not enough arguments\n");
			continue;
		}
		
		if (argc > 2) {
			printf("-rsh: cd: too many arguments\n");
			continue;
		}

		chdir(argv[1]);
	}
	else if (strcmp(argv[0], "exit")==0) return 0;
	else if (strcmp(argv[0], "help")==0) {
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

	else {
		// spawn cmds
		posix_spawnattr_init(&attr);
		// spawn
		if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
			perror("spawn failed");
			posix_spawnattr_destroy(&attr);
			continue;
		}
		// wait to terminate
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid failed");
		}
		// destroy spawned process
		posix_spawnattr_destroy(&attr);
	}
    }
    return 0;
}
