#include "types.h"
#include "user.h"

static int unint_data;
static int init_data = -1;
static const char const_data[] = "hello world";

int main(int argc, char *argv[], char *envp[]) {
    static int static_data = -2;

    int x = 3;
    char *theap = sbrk(0);

    printf(1, "location of envp[0]\t\t: %p\n", &(envp[0]));
    printf(1, "location of argv[0]\t\t: %p\n", &(argv[0]));

    printf(1, "location of stack\t\t: %p\n", &x);
    printf(1, "location of argc\t\t: %p\n", &argc);
    printf(1, "location of argv\t\t: %p\n", &argv);

    printf(1, "\n");

    printf(1, "location of heap\t\t: %p\n", malloc(1));
    printf(1, "location of sbrk(0)\t\t: %p\n", theap);
    printf(1, "\n");

    printf(1, "location of uninitialized data\t: %p\n", &unint_data);
    printf(1, "location of static data\t\t: %p\n", &static_data);
    printf(1, "location of initialized data\t: %p\n", &init_data);
    printf(1, "\n");

    printf(1, "location of const data\t\t: %p\n", &const_data);
    printf(1, "location of text\t\t: %p\n", main);

    exit();
}
