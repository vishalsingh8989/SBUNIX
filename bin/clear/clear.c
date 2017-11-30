#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>

int main(int argc, char* argv[], char* envp[]) {

	syscall_0(__NR_clearterm);
	return 0;
}
