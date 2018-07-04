#ifndef __STRING_H__
#define __STRING_H__

#ifndef NULL
#define NULL (0)
#endif

void *memset(void *s, int c, int count);
void *memcpy(void *dest, const void *src, int count);
int memcmp(const void *cs, const void *ct, int count);
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);

#endif