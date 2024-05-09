#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc != 2) // incorrect number of arguments
    {
        fprintf(2, "Usage: sleep ticks\n");
        exit(1);
    }

    sleep(atoi(argv[1])); // call sleep from user.h

    exit(0); // sleep success
}