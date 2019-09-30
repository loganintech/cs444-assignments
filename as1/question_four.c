#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
    {
        // child process
        printf("hello\n");
    }
    else
    {
        // parent process
        wait(NULL);
        printf("goodybye\n");
    }

    return 0;
}
