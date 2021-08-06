#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "logger.h"
#include "path.h"


extern void initPath(char path[], char *dest)
{
    char *user =getenv("USER");
    char msg[255];
    strcat(path, user);
    strcat(path, dest);
    strcpy(msg, path);
    struct stat sb;

    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        strcat(msg, " exists.");
        logger(INFO,msg);
    } else {
        strcat(msg, " doesn't exist, creating directory...");
        logger( INFO, msg);
        mkdir(path,0755);
        
    }
}