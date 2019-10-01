#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

int main()
{
    // Open a new file
    FILE *file = fopen("JUNK.txt", "w");

    // Write to a local buffer that will flush to the file from the OS
    // This buffer is copied to the child when forking so it will be flushed twice and end up in the file
    fprintf(file, "before fork\n");

    if (fork() == 0)
    {
        // In the child
        for (int i = 0; i < 10; i++)
        {
            // If i == 0 we're waiting for the parent to exit so that it will release the lock
            // on the filesystem because the OS won't let us concurrently write to the file
            // Queue up more messages for the child buffer
            fprintf(file, "child\n");
        }
    }
    else
    {
        // In the parent
        for (int i = 0; i < 10; i++)
        {
            // Queue up more messages for the parent buffer
            fprintf(file, "parent\n");
        }
    }

    // flush all of the buffers and write to the file before the process exits
    return 0;
}
