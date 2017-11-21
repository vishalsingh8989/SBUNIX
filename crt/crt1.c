#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/defs.h>

void _start() {

	uint64_t *sp_addr;
	//uint64_t offset = 101; //103 for O0
	uint64_t offset = 0x318/8; //103 for O0

	__asm__ __volatile__("movq %%rsp,  %0;"
			:"=r"(sp_addr)
			:
			:"rsp"
	);

	int argc = (int) *(sp_addr + offset);
	//int argc = (int) *sp_addr;
	char* argv[32] = { NULL };
	char* envp[64] = { NULL };

	//TODO: do dynamic memory allocation to get rid of this limitation.
	if (argc > 32) {
		puts("Sbush only supports 32 arguments to main!");
		exit(1);
	}

	for (int i = 0; i < argc; i++) {
		argv[i] = (char *) *(sp_addr + offset + i + 1);
	}

	char *temp = (char *) *(sp_addr + offset + argc + 2);
	int idx = 1;
	while (temp != 0 && idx < 64) {
		envp[idx - 1] = temp;
		idx++;
		temp = (char *) *(sp_addr + offset + argc + idx + 2);
	}

	int return_code;
	return_code = main(argc, argv, envp);
	exit(return_code);
}
