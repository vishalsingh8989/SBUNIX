#ifndef __KPRINTF_H
#define __KPRINTF_H

#include <sys/defs.h>

#define EN_INFO  0
#define EN_IMP   0
#define EN_ERR   0
#define EN_FATAL 1

#define INFO  0
#define IMP   1
#define ERR   2
#define FATAL 3
#define BOOTLOG 4

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
#define DEFAULT_COLOR    7

#define MAX_X   80
#define MAX_Y   23
#define STATUS  24

#define BACKSPACE  8
#define ENTER     10

uint64_t pnum_xy (uint64_t value, int base, char color, int x, int y);
void puts_xy(const char *str, char color, int x, int y);
void pchar_xy (char value, char color, int x, int y);
void pchar (char value);
void pstring (char* value);
void pnum (uint64_t value, int base);
void kprintf(const char *fmt, ...);
void klog(int severity, const char *fmt, ...);
void reset_cord();
void update_cursor(int x, int y);
void move_cursor_up();
#endif
