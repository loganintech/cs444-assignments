#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

    int xx = 100;

    // When calling fork xx is copied to the stack of the child thread
    if (fork() == 0)
    {
        // child process
        // The value of child's xx at this point is 100
        printf("xx child before change: %d\n", xx);
        xx = 777;
        // Now it's 777
        printf("xx child after change: %d\n", xx);
    }
    else
    {
        //parent process
        // The value of parent's xx at this point is 100
        printf("xx parent before change: %d\n", xx);
        xx = 999;
        // Now it's 999
        printf("xx parent after change: %d\n", xx);
    }

    return 0;
}
