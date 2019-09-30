#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
    {
        // child process

        // Passes params to the func from the arbitrarily-long list of arguments
        // Replaces current process context with `ls`, and returns with a failure since ls isn't in the local dir (path not considered)
        if (execl("ls", "-l", "-F", "-h", NULL) == -1)
        {
            printf("Execl had an error.\n");
        }

        // Passes params to the func (including funcs from path) from the arbitrarily-long list of arguments
        // Replaces current process context with `ls`, never returns, since it doesn't fail
        if (execlp("ls", "-l", "-F", "-h", NULL) == -1)
        {
            printf("Execlp had an error.\n");
        }

        char *args[4];
        args[0] = (char *)"-l";
        args[1] = (char *)"-F";
        args[2] = (char *)"-h";
        args[3] = NULL;

        // Passes params to the func from an array of null-terminated strings
        // Replaces current process context with `ls`, and returns with a failure since ls isn't in the local dir (path not considered)
        if (execv("ls", args) == -1)
        {
            printf("Execv had an error.\n");
        }

        // Replaces current process context with `ls`, never returns, since it doesn't fail
        // Passes params to the func (including funcs from path) from an array of null-terminated strings
        if (execvp("ls", args) == -1)
        {
            printf("Execvp had an error.\n");
        }
    }
    else
    {
        //parent process
        // Wait for the child process to exit
        wait(NULL);
    }

    return 0;
}
