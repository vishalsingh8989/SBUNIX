#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>

ssize_t write(int fd, const void *buf, size_t count) {
    uint64_t wr_cnt;
    wr_cnt = syscall_3(__NR_write, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
    return (ssize_t) wr_cnt;
}
