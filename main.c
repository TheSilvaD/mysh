#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>

void cd(char **args) {
	if (args[1] == NULL)
		chdir(getenv("HOME"));
	else
		if (chdir(args[1]) != 0)
			perror("sh");
}

char *readline(void){
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
	}
}

char **tokenizeline(char *line) {
	int buffersize = 64;
	int pos = 0;
	char **buffer = malloc(sizeof(char) *buffersize);
	char *token;

	token = strtok(line, " \n\a\r\t");
	while(token != NULL) {
		//printf("%s\n",token);
		buffer[pos] = token;
		pos++;
		token = strtok(NULL, " \n\a\r\t");
	}
	buffer[pos] = NULL;
	return buffer;
}

void executeargs(char **args){
	if (args[0] == NULL)
		return;
	if (strcmp(args[0], "exit") == 0)
		exit(EXIT_SUCCESS);
	if(strcmp(args[0], "cd") == 0) {
		cd(args);
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

void shell_loop(void) {
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
		line = readline();
		args = tokenizeline(line);
		executeargs(args);

		free(line); //prevents memory leak
		free(args);
	}
}

int main(int argc, char **argv) {
	shell_loop();
	return EXIT_SUCCESS;
}