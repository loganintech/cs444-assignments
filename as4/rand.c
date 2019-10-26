#include "rand.h"

int rand(void)
{
    rand_next = rand_next * 1103515245 + 12345;
    return ((unsigned)(rand_next / 65536) % RAND_MAX);
}

void srand(unsigned int seed)
{
    rand_next = seed;
}
