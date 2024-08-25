#include <signal.h>
#include <sys/wait.h>
#include <cassert>
#include <stdlib.h>
#include <signal.h>

void cgi(void) {
	int cgi_input[2], cgi_output[2];

	if (pipe(cgi_input) < 0 || pipe(cgi_output) < 0) {
		perror("pipe error");
		exit(1);
	}
	int pid = fork();
	if (pid < 0) {
		perror("fork error");
		exit(1);
	}
}