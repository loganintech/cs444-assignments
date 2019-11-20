#include "types.h"
#include "stat.h"
#include "user.h"
#include "benny_thread.h"

#ifdef KTHREADS
void func1(void *arg);

#ifndef DEFAULT_NUM_THREADS
# define DEFAULT_NUM_THREADS 2L
#endif // DEFAULT_NUM_THREADS

#ifndef MAX_THREADS
# define MAX_THREADS 20
#endif // MAX_THREADS

static benny_thread_t bt[MAX_THREADS] = {0};

void func1(void *);
void func2(void *);
void func3(void *);
void func4(void *);
void func5(void *);
void long_time(long);

void
func1(void *arg_ptr)
{
    long arg = ((long) arg_ptr);

    benny_thread_create(&(bt[arg]), func2, (void *) (arg + 1));

    long_time(arg);

    benny_thread_join(bt[3]); // tid = 4
    benny_thread_join(bt[4]); // tid = 5

    bt[3] = NULL;
    bt[4] = NULL;

    benny_thread_exit(1);
}

void
func2(void *arg_ptr)
{
    long arg = ((long) arg_ptr);

    benny_thread_create(&(bt[arg]), func3, (void *) (arg + 1));

    long_time(arg);

    benny_thread_exit(2);
}

void
func3(void *arg_ptr)
{
    long arg = ((long) arg_ptr);

    benny_thread_create(&(bt[arg]), func4, (void *) (arg + 1));

    long_time(arg);

    benny_thread_exit(3);
}

void
func4(void *arg_ptr)
{
    long arg = ((long) arg_ptr);

    benny_thread_create(&(bt[arg]), func5, (void *) (arg + 1));
    long_time(arg);

    benny_thread_exit(4);
}

void
func5(void *arg_ptr)
{
    long arg = ((long) arg_ptr);

    long_time(arg);

    benny_thread_exit(5);
}

void
long_time(long arg)
{
    int i = 0;
    uint max = MAXINT;
    uint sum = 1;

    // this objective for this fuction is to run for some period of time,
    // not toooooo long, but a bit.
    for(i = 0; i < max; i++) {
        sum ++;
        if ((sum % (MAXSHORT * MAXSHORT)) == 0) {
            sum += arg;
            printf(1, "\t%d  thtst2: %d  %d\n\n", arg, getpid(), sum);
        }
        if (sum > (MAXINT / 2)) {
            sum = 0;
        }
    }    
}
#endif // KTHREADS

int
main(int argc, char **argv)
{
#ifdef KTHREADS
    benny_thread_create(&(bt[0]), func1, (void *) 1);

    long_time(20);

    benny_thread_join(bt[0]); // tid = 1
    benny_thread_join(bt[1]); // tid = 2
    benny_thread_join(bt[2]); // tid = 3

    printf(1, "All threads joined\n");
#endif // KTHREADS
    exit();
}
