#include "types.h"
#include "user.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf(2, "Usage: renice [1 <= nice_val <= 40] [pid,]\n");
        exit();
    }

    int nice_val = atoi(argv[1]);
    if (nice_val < 1 || nice_val > 40)
    {
        printf(2, "Usage: renice [1 <= nice_val <= 40] [pid,]\n");
        exit();
    }

    for (int i = 2; argv[i] != NULL; i++)
    {
        int pid = atoi(argv[i]);
        if (pid == 0)
        {
            printf(2, "You must use a non-zero numberical PID to set nice values.\n");
            exit();
        }
        renice(nice_val, pid);
    }

    exit();
}
