#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024

char *shell_loop(void) {
	int buffer_size = BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * buffer_size);
	int c;
	printf("@ ");
	while(1){
		c = getchar();
		if(c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		}
		else {
			buffer[position] = c;
		}
		position++;
	}

}
int main(int argc, char **argv) {
	while(1) {
		printf(shell_loop());

	}
	return EXIT_SUCCESS;
}