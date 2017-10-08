#ifndef __KPRINTF_H
#define __KPRINTF_H

#include <sys/defs.h>
#define BLACK            0
#define BLUE             1
#define GREEN            2
#define CYAN             3
#define RED              4
#define MAGENTA          5
#define BROWN            6
#define LIGHT_GRAY       7
#define DARK_GRAY        8
#define LIGHT_BLUE       9
#define LIGHT_GREEN     10
#define LIGHT_CYAN      11
#define LIGHT_REG       12
#define LIGHT_MAGENTA   13
#define YELLO           14
#define WHITE           15

#define MAX_X   80
#define MAX_Y   25

#define MAX_X   80
#define MAX_Y   25

void pchar_xy (char value, char color, int x, int y);
void pchar (char value);
void pstring (char* value);
void pnum (uint64_t value, int base);

void kprintf(const char *fmt, ...);
void cls();
#endif
