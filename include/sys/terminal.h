#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <sys/defs.h>

void upd_term_buf(char c);
void term_read(uint64_t addr, uint64_t size);
void term_write(uint64_t addr, uint64_t size);

#endif
