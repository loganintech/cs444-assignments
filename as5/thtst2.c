#include "types.h"
#include "stat.h"
#include "user.h"
#include "benny_thread.h"

#ifdef KTHREADS

void func1(void *arg);

#ifndef DEFAULT_NUM_THREADS
# define DEFAULT_NUM_THREADS 4L
#endif // DEFAULT_NUM_THREADS

#ifndef MAX_THREADS
# define MAX_THREADS 20
#endif // MAX_THREADS

void 
func1(void *arg_ptr)
{
    long arg = ((long) arg_ptr);
    int i = 0;
    uint max = MAXINT;
    uint sum = 1;

    // this objective for this fuction is to run for some period of time,
    // not toooooo long, but a bit.
    for(i = 1; i < max; i++) {
        sum ++;
        if ((sum % (MAXSHORT * MAXSHORT)) == 0) {
            sum += arg;
            printf(1, "\t%d  thtst2: %d  %d\n\n", arg, getpid(), sum);
        }
        if (sum > (MAXINT / 2)) {
            sum = 0;
        }
    }

    benny_thread_exit(7);
}
#endif // KTHREADS

int
main(int argc, char **argv)
{
#ifdef KTHREADS
    benny_thread_t bt[MAX_THREADS];
    long i = -1;
    long num_threads = DEFAULT_NUM_THREADS;

    //printf(1, "%s %d: %p %p\n", __FILE__, __LINE__, main, &argc);
    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads < 1 || num_threads > MAX_THREADS) {
            num_threads = DEFAULT_NUM_THREADS;
        }
    }
    printf(1, "Starting %d threads\n", num_threads);
    
    for (i = 0; i < num_threads; i++) {
        benny_thread_create(&(bt[i]), func1, (void *) i);
        printf(1, "%s %d: started thread %d\n", __FILE__, __LINE__, benny_thread_tid(bt[i]));
    }

    for (i = 0; i < num_threads; i++) {
        printf(1, "%s %d: joining with %d\n", __FILE__, __LINE__, benny_thread_tid(bt[i]));
        benny_thread_join(bt[i]);
    }

    printf(1, "All threads joined\n");
#endif // KTHREADS
    exit();
}
