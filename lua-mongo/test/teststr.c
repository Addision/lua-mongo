#include <stdio.h>
#include <limits.h>
#define point2uint(p)	((unsigned int)((size_t)(p) & UINT_MAX))

int main()
{
    const char *test = "hahaxixix";
    print("%ld\n", point2uint(test));
    unsigned int i = point2uint(test) % 53;
    print("%d\n", i);
    return 0;
}
