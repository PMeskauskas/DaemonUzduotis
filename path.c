#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "logger.h"
#include <signal.h>
#include "path.h"

extern void pathInit(char path[], char *dest)
{
    char *user = getenv("USER");
    strcat(path, user);
    strcat(path, dest);

    pathExists(path);
}

extern void pathExists(char path[])
{
    struct stat sb;
    char msg[256];
    strcpy(msg, path);
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        strcat(msg, " exists.");
        logger(INFO, msg);
    }
    else
    {
        strcat(msg, " doesn't exist, creating directory...");
        logger(INFO, msg);
        if(mkdir(path, 0755) != -1){
            logger(ERROR, "Failed to make directory, exiting...");
            exit(1);
        }
    }
}
extern void pathCheck(char path[], pid_t process_id)
{
    struct stat sb;
    char msg[256];
    strcpy(msg, path);
    if (stat(path, &sb) != 0 && S_ISDIR(sb.st_mode))
    {
        strcat(msg, " doesn't exist anymore, shutting down...");
        logger(ERROR, msg);
        kill(process_id, SIGKILL);
    }
}