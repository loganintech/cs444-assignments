#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
    {
        // child process
        char *args[3];
        args[0] = (char *)"-l";
        args[1] = (char *)"-F";
        args[2] = (char *)"-h";
        // Passes params to the func from the arbitrarily-long list of arguments
        if (execl("ls", "-l", "-F", "-h", NULL) == -1)
        {
            printf("Execl had an error.\n");
        }
        // Passes params to the func (including funcs from path) from the arbitrarily-long list of arguments
        if (execlp("ls", "-l", "-F", "-h", NULL) == -1)
        {
            printf("Execlp had an error.\n");
        }
        // Passes params to the func from an array of null-terminated strings
        if (execv("ls", args) == -1)
        {
            printf("Execv had an error.\n");
        }
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
