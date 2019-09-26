#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

int main()
{

    // Open a new file
    FILE *file = fopen("JUNK.txt", "w");

    // Write to a local buffer that will flush to a file from the OS
    fprintf(file, "before fork\n");

    // Fork, copying the local `before fork` buffer to the child thread
    if (fork() == 0)
    {
        // In the child
        for(int i = 0; i < 10; i++) {
            // Queue up more messages for the child buffer
            fprintf(file, "child\n");
        }
    }
    else
    {
        // In the parent
        for(int i = 0; i < 10; i++) {
            // Queue up more messages for the parent buffer
            fprintf(file, "parent\n");
        }
    }

    // When the child / parent finishes execution, flush the output
    // Typically parent will finish first since the CPU would have to context-switch over to the child...
    // ... which means that the parent will usually print first.
    return 0;
}
