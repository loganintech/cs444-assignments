#include "types.h"
#include "user.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf(2, "Usage: nice [1 <= nice_val <= 40] [command].\n");
        exit();
    }

    int nice_val = atoi(argv[1]);
    if (nice_val < 1 || nice_val > 40)
    {
        printf(2, "Usage: nice [1 <= nice_val <= 40] [command].\n");
        exit();
    }

    int pid = getpid();
    if (renice(nice_val, pid) < 0)
    {
        printf(2, "Renice failed.");
        exit();
    }

    exec(argv[2], &argv[2]);

    exit();
}
