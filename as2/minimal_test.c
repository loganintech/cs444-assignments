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

    void *someptr = beavalloc(2048);
    beavfree(someptr);
    someptr = beavalloc(50);
    void *someptr2 = beavalloc(200);
    void *someptr3 = beavalloc(220);
    beavfree(someptr2);
    someptr2 = beavalloc(50);
    void *someptr4 = beavalloc(60);
    beavfree(someptr3);
    beavfree(someptr4);
    beavfree(someptr);
    beavalloc_dump(FALSE);
}
