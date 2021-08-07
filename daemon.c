#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "logger.h"
extern void initDaemon(pid_t process_id)
{

pid_t sid = 0;
process_id = fork();
if (process_id < 0)
{
    printf("fork failed!\n");
    exit(1);
}
if (process_id > 0)
{
    printf("process_id of child process %d \n", process_id);
    exit(0);
}
umask(0);
sid = setsid();
if(sid < 0)
{
    exit(1);
}
chdir("/");
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
}