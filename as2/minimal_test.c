#include "beavalloc.h"
#include "assert.h"

int main()
{
    void *otherptr = beavalloc(1024);
    beavfree(otherptr);
    void *smallptr = beavalloc(256);
    void *smallptr2 = beavalloc(512);
    printf("%p, %p, %p\n", otherptr, smallptr, smallptr2);
}
