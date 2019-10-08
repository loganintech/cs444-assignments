#include "beavalloc.h"
#include "assert.h"

int main()
{
    void *ptr = beavalloc(20);
    void *ptr2 = beavalloc(30);
    void *ptr3 = beavalloc(40);
    void *ptr4 = beavalloc(50);
    beavalloc_dump(FALSE);
}
