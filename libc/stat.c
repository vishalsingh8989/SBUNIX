#include <stdio.h>
#include <unistd.h>
#include <sys/defs.h>
#include <sys/stat.h>
#include <sys/syscall.h>

fstat_t* stat(int fidx,fstat_t* statbuf){

	   uint64_t out;
	   out = syscall_2(__NR_fstat, (uint64_t) fidx, (uint64_t) statbuf);

	   return (fstat_t*)out;
}
