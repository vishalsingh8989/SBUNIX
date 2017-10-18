#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_



typedef struct timespec {
        uint32_t   tv_sec;        /* seconds */
        uint64_t     tv_nsec;       /* nanoseconds */
}timespec;
//void *memset(void *d , uint32_t to,  uint64_t num);
//void *memcopy(void *d, const void *s, uint64_t num);
void *memset(void *dest, char val, size_t count);
void dsleep();
#endif
