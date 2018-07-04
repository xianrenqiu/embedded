#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#define assert(cond)                                                                  \
    do                                                                                \
    {                                                                                 \
        if (!(cond))                                                                  \
        {                                                                             \
            printf("assert: '" #cond "' failed [%s line: %d]\n", __FILE__, __LINE__); \
            return -1;                                                                \
        }                                                                             \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
