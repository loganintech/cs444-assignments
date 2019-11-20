#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
#ifdef KDEBUG
    int val = 0;

    if (argc > 1) {
        val = atoi(argv[1]);
    }

    kdebug(val);
#endif // KDEBUG

    exit();
}
