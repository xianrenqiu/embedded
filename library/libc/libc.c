#include <stdarg.h>

/**
 *
 * @brief Compare two strings
 *
 * @return negative # if <s1> < <s2>, 0 if <s1> == <s2>, else positive #
 */

int strcmp(const char *s1, const char *s2)
{
    while ((*s1 == *s2) && (*s1 != '\0')) {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void *cs, const void *ct, int count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;

    return res;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *memcpy(void *dest, const void *src, int count)
{
    char *tmp = dest;
    const char *s = src;
 
    while (count--)
        *tmp++ = *s++;

    return dest;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void *memset(void *s, int c, int count)
{
    char *xs = s;

    while (count--)
        *xs++ = c;
    return s;
}

/**
 *
 * @brief Get string length
 *
 * @return number of bytes in string <s>
 */

int strlen(const char *s)
{
	int n = 0;

	while (*s != '\0') {
		s++;
		n++;
	}

	return n;
}

struct printf_info 
{
    char *bf; /* Digit buffer */
    char zs; /* non-zero if a digit has been written */
    char *outstr; /* Next output position for sprintf() */    
    void (*putc)(struct printf_info *info, char ch); /* Output a character */
};

static void putc_normal(struct printf_info *info, char ch)
{
    extern void uart_putc(unsigned char data);
    uart_putc(ch);
}

static void out(struct printf_info *info, char c)
{
    *info->bf++ = c;
}
 
static void out_dgt(struct printf_info *info, char dgt)
{
    out(info, dgt + (dgt < 10 ? '0' : 'a' - 10));
    info->zs = 1;
}
 
static void div_out(struct printf_info *info, unsigned int *num, unsigned int div)
{
    unsigned char dgt = 0;
 
    while (*num >= div) 
    {
        *num -= div;
        dgt++;
    }
 
    if (info->zs || dgt > 0)
        out_dgt(info, dgt);
}
 
static int _vprintf(struct printf_info *info, const char *fmt, va_list va)
{
    char ch;
    char *p;
    unsigned int num;
    char buf[12];
    unsigned int div;
 
    while ((ch = *(fmt++))) 
    {
        if (ch != '%') 
        {
            info->putc(info, ch);
        } 
        else 
		{
            int lz = 0;
            int width = 0;
 
            ch = *(fmt++);
            if (ch == '0') 
			{
                ch = *(fmt++);
                lz = 1;
            }
 
            if (ch >= '0' && ch <= '9') 
			{
                width = 0;
                while (ch >= '0' && ch <= '9') 
				{
                    width = (width * 10) + ch - '0';
                    ch = *fmt++;
                }
            }
            info->bf = buf;
            p = info->bf;
            info->zs = 0;
 
            switch (ch) 
			{
            case '\0':
                goto abort;
            case 'u':
            case 'd':
                num = va_arg(va, unsigned int);
                if (ch == 'd' && (int)num < 0) 
				{
                    num = -(int)num;
                    out(info, '-');
                }
                if (!num) 
				{
                    out_dgt(info, 0);
                } 
				else 
				{
                    for (div = 1000000000; div; div /= 10)
                        div_out(info, &num, div);
                }
                break;
            case 'x':
                num = va_arg(va, unsigned int);
                if (!num) 
				{
                    out_dgt(info, 0);
                } 
				else 
				{
                    for (div = 0x10000000; div; div /= 0x10)
                        div_out(info, &num, div);
                }
                break;
            case 'c':
                out(info, (char)(va_arg(va, int)));
                break;
            case 's':
                p = va_arg(va, char*);
                break;
            case '%':
                out(info, '%');
            default:
                break;
            }
 
            *info->bf = 0;
            info->bf = p;
            while (*info->bf++ && width > 0)
                width--;
            while (width-- > 0)
                info->putc(info, lz ? '0' : ' ');
            if (p) 
			{
                while ((ch = *p++))
                    info->putc(info, ch);
            }
        }
    }
 
abort:
    return 0;
}
 
int printf(const char *fmt, ...)
{
    int ret;
    va_list va;
    struct printf_info info;
 
    info.putc = putc_normal;
    va_start(va, fmt);
    ret = _vprintf(&info, fmt, va);
    va_end(va);
 
    return ret;
}
