#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

#ifdef KTHREADS

#include "benny_thread.h"

#ifndef PGSIZE
#define PGSIZE 4096
#endif // PGSIZE

struct benny_thread_s
{
    int tid;
    void *tstack;
};

extern int kthread_create(void (*func)(void *), void *, void *);
extern int kthread_join(benny_thread_t);
extern void kthread_exit(int);

int benny_thread_create(benny_thread_t *vbt, void (*func)(void *), void *arg_ptr)
{
    struct benny_thread_s *bt = malloc(sizeof(struct benny_thread_s));
    void *tstack = NULL;

    bt->tstack = tstack = malloc(PGSIZE * 2);
    if (tstack == NULL)
    {
        return -1;
    }
    if (((uint)tstack) % PGSIZE != 0)
    {
        tstack += (PGSIZE - ((uint)tstack) % PGSIZE);
    }
    bt->tid = kthread_create(func, arg_ptr, tstack);
    sleep(1);
    printf(1, "\n%s %d: new thread %d\n", __FILE__, __LINE__, bt->tid);

    if (bt->tid != 0)
    {
        *vbt = (benny_thread_t)bt;
        return 0;
    }
    return -1;
}

int benny_thread_tid(benny_thread_t vbt)
{
    struct benny_thread_s *bt = (struct benny_thread_s *)vbt;

    return bt->tid;
}

int benny_thread_join(benny_thread_t vbt)
{
    struct benny_thread_s *bt = (struct benny_thread_s *)vbt;
    int retVal = -1;

    retVal = kthread_join(bt->tid);
    if (retVal == 0)
    {
        free(bt->tstack);
    }

    return retVal;
}

int benny_thread_exit(int exitValue)
{
    kthread_exit(exitValue);
    return 0;
}

#endif // KTHREADS
