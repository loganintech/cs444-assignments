#include "beavalloc.h"
#include "assert.h"

int main()
{
    void *otherptr = beavalloc(1024);
    beavalloc_dump(FALSE);
    beavfree(otherptr);
    void *smallptr = beavalloc(256);
    beavalloc_dump(FALSE);
    void *smallptr2 = beavalloc(512);
    beavalloc_dump(FALSE);
    beavfree(smallptr2);
    beavalloc_dump(FALSE);
    beavfree(smallptr);
    beavalloc_dump(FALSE);
    printf("%p, %p, %p\n", otherptr, smallptr, smallptr2);
}
