#include<sys/defs.h>

/*
 *
 * set mem to 'to'
 */
void *memset(void *dest , uint32_t to,  uint64_t num){
	 	unsigned char* mem=dest;
	    while(num--)
	        *mem++ = (unsigned char)to;
	    return dest;
}


/*
 *copy data from s to  d
 *
 */
void *memcopy(void *d, const void *s, uint64_t num){

	const char *mem_s = s;
    char *mem_d = d;
    for (uint64_t i = 0; i < num; i++) {
        *mem_d++ = *mem_s++;
    }
    return d;
}
