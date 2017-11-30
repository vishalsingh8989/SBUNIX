#include <sys/time.h>
#include <sys/syscall.h>


uint64_t gettime(struct tm* tm_time){

	uint64_t out;

	out = syscall_1(__NR_gettimeofday, (uint64_t)tm_time);

	return out;
}
