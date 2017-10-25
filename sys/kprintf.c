#include <sys/kprintf.h>
#include <sys/defs.h>
#include <stdarg.h>

static int x_cord = 0;
static int y_cord = 0;

static uint64_t video_p = 0xffffffff800b8000;

void scroll() {
    char *disp;

    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            disp = (char *) (video_p + 2*(MAX_X*i + j));
            if (i == 2*MAX_Y-1) *(disp + 1) = BLACK;
            else {
                *disp = *(disp + 2*MAX_X);
                *(disp+1) = *(disp + 2*MAX_X + 1);
            }
            
        }
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

void pchar_xy (char value, char color, int x, int y) 
{
    char *disp;
    disp = (char *) (video_p + 2*(MAX_X*y + x));

    *disp     = value;
    *(disp+1) = color;
}

void pchar (char value) //TODO: support tab characters
{
    if(value != '\n' &&
       value != '\r') {
        pchar_xy(value, LIGHT_GRAY, x_cord, y_cord);
    }

    if(value == '\t') {
        int temp = ((x_cord + 4) <= MAX_X) ? 4 : MAX_X - x_cord;
        for(int i = 0; i < temp ; i++) {
            pchar_xy(' ', LIGHT_GRAY, x_cord, y_cord);
            x_cord++;
        }
        return;
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
