#include<sys/defs.h>
#include<sys/data_utils.h>
#include<sys/pci.h>
#include<sys/kprintf.h>


//! sets count bytes of dest to val
void *memset(void *dest, char val, size_t count)
{
    unsigned char *temp = (unsigned char *)dest;
	for( ; count != 0; count--, temp[count] = val);
	return dest;
}


void dsleep()
{
	//inefficient sleep
	for(uint64_t sl = 0 ; sl <99999999 ;sl++ );
	}
/*
 *
 * set mem to 'to'  not working
 */
//void *memset(void *dest , uint32_t frm,  uint64_t num){
//
//		unsigned char* mem=dest;
//
//	    while(num--){
//	    		//kprintf("memset 3 %p  == %p, %d\n", mem,frm, num);
//	    		//dsleep();
//	        *mem++ = (unsigned char)frm;
//	    }
//	    return dest;
//}
//
//
///*
// *copy data from s to  d
// *
// */
//void *memcopy(void *d, const void *s, uint64_t num){
//
//	const char *mem_s = s;
//    char *mem_d = d;
//    for (uint64_t i = 0; i < num; i++) {
//        *mem_d++ = *mem_s++;
//    }
//    return d;
//}
