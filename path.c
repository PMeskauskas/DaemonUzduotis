#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path.h"

extern char *initPath(char path[], char *dest)
{
    char *user =getenv("USER");
    strcat(path, user);
    strcat(path, dest);
    return path;
}