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
        printf("execl: ");
        if (execl("ls", "-l", "-F", "-h", NULL) == -1)
        {
            printf("Execl had an error.\n");
        }

        // Passes params to the func (including funcs from path) from the arbitrarily-long list of arguments
        // Replaces current process context with `ls`, never returns, since it doesn't fail
        printf("execlp: ");
        fflush(0);
        if (execlp("ls", "-l", "-F", "-h", NULL) == -1)
        {
            printf("Execlp had an error.\n");
        }

        // *****************************************************
        // NOTHING BELOW HERE RUNS BECAUSE EXECLP IS SUCCESSFUL
        // *****************************************************

        // prep the args
        char *args[4];
        args[0] = (char *)"-l";
        args[1] = (char *)"-F";
        args[2] = (char *)"-h";
        args[3] = NULL;

        // Passes params to the func from an array of null-terminated strings
        // Replaces current process context with `ls`, and returns with a failure since ls isn't in the local dir (path not considered)
        printf("execv: ");
        if (execv("ls", args) == -1)
        {
            printf("Execv had an error.\n");
        }

        // Replaces current process context with `ls`, never returns, since it doesn't fail
        // Passes params to the func (including funcs from path) from an array of null-terminated strings
        printf("execvp: ");
        fflush(0);
        if (execvp("ls", args) == -1)
        {
            printf("Execvp had an error.\n");
        }
    }
    else
    {
        // parent process
        // wait for the processes to exit
        wait(NULL);
    }

    return 0;
}
