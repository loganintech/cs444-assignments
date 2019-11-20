#include "types.h"
#include "stat.h"
#include "user.h"
#include "benny_thread.h"

#ifdef KTHREADS
void func1(void *arg);

static int global = 10;

#define IVAL 0x0f0f0f0f
#define FVAL 0Xaeaeaeae

void 
func1(void *arg)
{
    int *i = (int *) ((int *) arg);

    //printf(1, "%s %d\n", __FILE__, __LINE__);
    assert(global == 10);
    assert(*i == IVAL);

    //printf(1, "%s %d\n", __FILE__, __LINE__);

    *i = FVAL;
    global = 100;

    //printf(1, "%s %d\n", __FILE__, __LINE__);
    benny_thread_exit(3);
}
#endif // KTHREADS

int
main(int argc, char **argv)
{
#ifdef KTHREADS
    benny_thread_t bt = NULL;
    int rez = -1;
    int i = IVAL;
    
    printf(1, "global before: %d\n", global);
    printf(1, "i before     : %x\n", i);
    rez = benny_thread_create(&bt, func1, &i);

    sleep(2);

    printf(1, "rez          : %x\n", rez);

    rez = benny_thread_join(bt);
    printf(1, "global after : %d\n", global);
    printf(1, "i after      : %x\n", i);
    printf(1, "rez          : %d\n", rez);
    assert(global == 100);
    assert(rez == 0);
    assert(i == FVAL);

    sleep(2);
#endif // KTHREADS
    exit();
}
