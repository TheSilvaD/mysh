#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>

void changeDirectory(char **args) {
	if (args[1] == NULL)
		chdir(getenv("HOME"));
	else
		if (chdir(args[1]) != 0)
			perror("sh");
}

char *readLine(void){
	int buffersize = 1024;
	int pos = 0;
	char *buffer = malloc(sizeof(char)*buffersize);
	int c;

	while(1) {
		c = getchar();
		//checks to see if we hit EOF or ENTER key
		if (c == EOF || c =='\n') {
			buffer[pos] = '\0';
			return buffer;
		}
		else buffer[pos] = c; //fills array with each char as an int
		pos++;

		if (pos >= buffersize) {
			buffersize += 1024;
			buffer = realloc(buffer, buffersize);
		}
	}
}

char **tokenizeLine(char *line) {
	int buffersize = 64;
	int pos = 0;
	char **buffer = malloc(sizeof(char*)*buffersize);
	char *token;

	token = strtok(line, " \n\a\r\t");
	while(token != NULL) {
		//printf("%s\n",token);
		buffer[pos] = token;
		pos++;

		if (pos >= buffersize) {
			buffersize += 64;
			buffer = realloc(buffer, buffersize * sizeof(char*));
		}
		token = strtok(NULL, " \n\a\r\t");
	}
	buffer[pos] = NULL;
	return buffer;
}

void executeArgs(char **args){
	if (args[0] == NULL)
		return;
	if (strcmp(args[0], "exit") == 0)
		exit(EXIT_SUCCESS);
	if(strcmp(args[0], "cd") == 0) {
		changeDirectory(args);
		return;
	}
	pid_t pid, wpid;
	int status;
	
	pid = fork();

	if (pid == 0) {
    // Child process
		if (execvp(args[0], args) == -1)
    		perror("sh");
    	exit(EXIT_FAILURE);
    } else if (pid < 0) {
    	// Error forking
    	perror("sh");
    } else {
    // Parent process
    	do
    		wpid = waitpid(pid, &status, WUNTRACED);
    	while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void shellLoop(void) {
	//read line
	//tokenize line into args
	//execute args
	char *cwd;
	char buff[PATH_MAX];
	char *line;
	char **args;

	while(1) {
		cwd = getcwd(buff, PATH_MAX);
		printf("[%s]@ ",basename(cwd));
		line = readLine();
		args = tokenizeLine(line);
		executeArgs(args);

		free(line); //prevents memory leak
		free(args);
	}
}

int main(int argc, char **argv) {
	shellLoop();
	return EXIT_SUCCESS;
}