#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/utils.h>
#include <sys/asm_utils.h>
#include <sys/env.h>
#include <stdarg.h>

static int x_cord = 0;
static int y_cord = 0;

//TODO: change to define.
char* video_p = (char *) 0xffffffff800b8000;
char term_color = LIGHT_GRAY;
void reset_cord(){
	x_cord = 0;
	y_cord = 0;
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * MAX_X + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void scroll() {
    for(int i = 0; i < 2*MAX_X; i++) {
        *(video_p + 2*MAX_X*(MAX_Y) + i) = BLACK;
    }
    memcpy(video_p, video_p+(2*MAX_X), (2*MAX_X*(MAX_Y)));
}

void kprintf_log(const char *fmt, va_list args)
{
    int idx = 0;

    while (*(fmt+idx) != '\0') {
        char temp1 = *(fmt+idx);
        char ctemp;
        char *stemp;
        int ntemp;
        uint64_t ptemp;

        if (temp1 == '%') {
            idx++;
            char temp2 = *(fmt+idx);
            switch (temp2) {
                case 'c' :
                    ctemp = va_arg(args, int);
                    pchar(ctemp);
                    break;
                case 'd' :
                    ntemp = va_arg(args, int);
                    if (ntemp < 0) {
                        pchar('-');
                        ntemp = -ntemp;
                    }
                    pnum(ntemp, 10);
                    break;
                case 'x' :
                    ptemp = va_arg(args, uint64_t);
                    pnum(ptemp, 16);
                    break;
                case 's' :
                    stemp = va_arg(args, char*);
                    pstring(stemp);
                    break;
                case 'p' :
                    ptemp = va_arg(args, uint64_t);
                    pstring("0x");
                    pnum(ptemp, 16);
                    break;
                default:
                    pstring("Invalid Format String: ");
                    pchar(temp2);
                    pchar('\n');
            }
        }
        else {
            pchar(temp1);
        }
        idx++;
    }
}

void klog(int severity, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if(severity == INFO && EN_INFO) {
        term_color = YELLO;
        kprintf("INFO>> ");
        kprintf_log(fmt, args);
        term_color = DEFAULT_COLOR;
    }
    else if(severity == IMP && EN_IMP) {
        term_color = CYAN;
        kprintf("IMP>> ");
        kprintf_log(fmt, args);
        term_color = DEFAULT_COLOR;
    }
    else if(severity == ERR && EN_ERR) {
        term_color = RED;
        kprintf("ERROR>> ");
        kprintf_log(fmt, args);
        term_color = DEFAULT_COLOR;
    }
    else if(severity == FATAL && EN_FATAL) {
        term_color = RED;
        kprintf("FATAL>> ");
        kprintf_log(fmt, args);
        term_color = DEFAULT_COLOR;
    }
}

void kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int idx = 0;

    while (*(fmt+idx) != '\0') {
        char temp1 = *(fmt+idx);
        char ctemp;
        char *stemp;
        int ntemp;
        uint64_t ptemp;

        if (temp1 == '%') {
            idx++;
            char temp2 = *(fmt+idx);
            switch (temp2) {
                case 'c' :
                    ctemp = va_arg(args, int);
                    pchar(ctemp);
                    break;
                case 'd' :
                    ntemp = va_arg(args, int);
                    if (ntemp < 0) {
                        pchar('-');
                        ntemp = -ntemp;
                    }
                    pnum(ntemp, 10);
                    break;
                case 'x' :
                    ptemp = va_arg(args, uint64_t);
                    pnum(ptemp, 16);
                    break;
                case 's' :
                    stemp = va_arg(args, char*);
                    pstring(stemp);
                    break;
                case 'p' :
                    ptemp = va_arg(args, uint64_t);
                    pstring("0x");
                    pnum(ptemp, 16);
                    break;
                default:
                    pstring("Invalid Format String: ");
                    pchar(temp2);
                    pchar('\n');
            }
        }
        else {
            pchar(temp1);
        }
        idx++;
    }

    va_end(args);
}

void puts_xy(const char *str, char color, int x, int y)
{
    int x_cord = x;
    int y_cord = y;

    while (*str != '\0') {
        if(x_cord > MAX_X-1) {
           klog(IMP, "put_xy() is going beyond screen dimension, please fix the calling coordinates\n");
           return;
        }
        pchar_xy(*str++, color, x_cord++, y_cord);
    }
}

void pchar_xy (char value, char color, int x, int y)
{
    char *disp;
    disp = (char *) (video_p + 2*(MAX_X*y + x));

    *disp     = value;
    *(disp+1) = color;
    update_cursor(x_cord ,y_cord);
}

void pchar (char value) //TODO: support tab characters
{

    if(value == '\t') {
        int temp = ((x_cord + 4) <= MAX_X) ? 4 : MAX_X - x_cord;
        for(int i = 0; i < temp ; i++) {
            pchar_xy(' ', term_color, x_cord, y_cord);
            x_cord++;
        }
        return;
    }

    int prompt_len = strlen(PWD) + strlen(PS1);

    if(value == BACKSPACE) {
        if(x_cord > prompt_len ) {
            x_cord--;
            pchar_xy(' ', term_color, x_cord, y_cord);
        }
        return;
    }

    if(value != '\n' &&
       value != '\r') {
        pchar_xy(value, term_color, x_cord, y_cord);
    }

    if(x_cord == MAX_X-1 && y_cord == MAX_Y-1) {
        x_cord = 0;
        scroll();
    }
    else if(y_cord == MAX_Y-1 && value == '\n') {
        x_cord = 0;
        scroll();
    }
    else if(x_cord == MAX_X-1 || value == '\n') {
        x_cord = 0;
        y_cord++;
    }
    else if(value == '\r') {
        x_cord = 0;
    }
    else {
        x_cord++;
    }
}

void pstring (char * value) {
    while (*value != '\0') {
        pchar(*value++);
    }
}

uint64_t pnum_xy (uint64_t value, int base, char color, int x, int y) {
    uint64_t x_cord = (uint64_t) x;
    if (value <= (base-1)) {
        if (value < 10) {
           pchar_xy((char) (value+48), color, x, y);
        }
        else {
           pchar_xy((char) (value+87), color, x, y);
        }
        return (x_cord+1);
    }
    else {
        x_cord = pnum_xy(value/base, base, color, x_cord, y);
        x_cord = pnum_xy(value - (value/base)*base, base, color, x_cord, y);
        return x_cord;
    }
}

void pnum (uint64_t value, int base) {
    if (value <= (base-1)) {
        if (value < 10) pchar((char) (value+48));
        else pchar((char) (value+87));
    }
    else {
        pnum(value/base, base);
        pnum(value - (value/base)*base, base);
    }
}
