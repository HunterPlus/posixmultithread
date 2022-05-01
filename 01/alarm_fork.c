#include <sys/types.h>
#include <wait.h>
#include "errors.h"

int main(int argc, char *argv[])
{
	int	status;
	char	line[128];
	int	seconds;
	pid_t	pid;
	char	message[64];
	
	while (1) {
		printf("Alarm> ");
		if (fgets(line, sizeof (line), stdin) == NULL)
			exit(0);
		if (strlen(line) <= 1)
			continue;
		if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2)
			fprintf(stderr, "Bad command\n");
		else {
			pid = fork();
			if (pid == (pid_t) -1)
				errno_abort("Fork");
			if (pid == (pid_t) 0) {
				/*
				 * In the child, wait and then print a message
				 */
				sleep(seconds);
				printf("%d %s\n", seconds, message);
				exit(0);
			} else {
				/* 
				 * In the parent, call waitpid() to collect children
				 * that have already terminate.
				 */
				do {
					pid = waitpid((pid_t) -1, NULL, WNOHAND);
					if (pid = (pid_t) -1)
						errno_abort("wait for child");
				} while (pid != (pid_t) 0);
			}
		}
	}
}
