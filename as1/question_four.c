#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;

int main()
{
    pthread_mutex_init(&lock, NULL);
    if (fork() == 0)
    {
        pthread_mutex_lock(&lock);
        // child process
        printf("hello\n");
        pthread_mutex_unlock(&lock);
    }
    else
    {
        // parent process
        sleep(1);
        pthread_mutex_lock(&lock);
        printf("goodybye\n");
        pthread_mutex_unlock(&lock);
    }

    pthread_mutex_destroy(&lock);

    return 0;
}
