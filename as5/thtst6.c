#include "types.h"
#include "stat.h"
#include "user.h"
#include "benny_thread.h"

#ifdef KTHREADS
void func1(void *arg);

extern int kthread_create(void (*func)(void *), void *, void *);
extern void kthread_exit(int);

void 
func1(void *arg)
{
    int i = (int) ((long) arg);


    kthread_exit(i);
}
#endif // KTHREADS

int
main(int argc, char **argv)
{
#ifdef KTHREADS
    char *ptr = malloc(20);
    int rez = -17;

    ptr = malloc(20);

    rez = kthread_create(func1, NULL, ptr);
    printf(1, "%s %d: %d\n", __FILE__, __LINE__, rez);
#endif // KTHREADS
    exit();
}
