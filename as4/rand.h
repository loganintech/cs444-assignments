#ifndef RAND
#define RAND

#define RAND_MAX (1 << 31)

int rand_next;

int rand(void);
void srand(unsigned int seed);

#endif
