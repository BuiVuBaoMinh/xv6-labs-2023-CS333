#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("sysinfo called in user mode.\n");
    struct sysinfo info;
    sysinfo(&info);
    printf("Free mem: %d Bytes\n", info.freemem);
    printf("Num proc: %d procs\n", info.nproc);
    exit(0);
}