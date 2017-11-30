
#include <sys/defs.h>
#include	 <sys/syscall.h>

uint64_t procps(){

	uint64_t out;
	out = syscall_0(__NR_ps);
	return (int) out;
}
