#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char **argv)
{
    void *ptr = NULL;

    ptr = malloc(50);
    ptr = malloc(PGSIZE * 2);
    printf(1, "%p ptr\n", ptr);
    printf(1, "%u pg offset ptr\n", ((ulong) ptr) % PGSIZE);

    ptr += (PGSIZE - ((ulong) ptr) % PGSIZE);
    printf(1, "%p ptr\n", ptr);
    printf(1, "%u pg offset ptr\n", ((ulong) ptr) % PGSIZE);

    exit();
}
