#include "bench.h"

static inline int rand_int(int minv, int maxv)
{
    return minv + rand() % (maxv - minv);
}



extern void rndmPass(char * const  pass, size_t len)
{
    /* ASCII Printable characters*/
    char const data[] = "!#$%&()*+,-.0123456789:;<=>?@"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]_^"
                        "abcdefghijklmnopqrstuvwxyz{|}~";

    size_t i=0, data_len = (size_t)(sizeof(data)-1);

    if(!len)
        return;

    for (; i < len; i++)
        pass[i] = data[rand_int(0, data_len)];
    pass[len] = '\0';
}
