#ifndef __BENNY_THREAD_H
# define __BENNY_THREAD_H

# ifdef KTHREADS

typedef ulong benny_thread_t;

int benny_thread_create(benny_thread_t *, void (*func)(void*), void *);
int benny_thread_join(benny_thread_t);
int benny_thread_exit(int);
int benny_thread_tid(benny_thread_t);

# endif // KTHREADS

#endif // __BENNY_THREAD_H
