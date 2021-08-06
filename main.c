#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "path.h"
#include "logger.h"
#include "daemon.h"
#include "search.h"
#define PATH_LENGTH 40
typedef enum LOCATION {
    DOWNLOADS ,MUSIC, VIDEOS, PICTURES, DOCUMENTS
} LOCATION;
char *DESTINATIONS[] = {
    "/Downloads","/Music", "/Videos", "/Pictures", "/Documents"
};
struct path {
    char downPath[PATH_LENGTH];
    char musicPath[PATH_LENGTH];
    char videoPath[PATH_LENGTH];
    char picPath[PATH_LENGTH];
    char docPath[PATH_LENGTH];
};
int main(void)
{
    struct path paths = {
        "/home/", "/home/", "/home/", "/home/","/home/"  
        };
    open_log();
    initPath(paths.downPath, DESTINATIONS[0]);
    initPath(paths.musicPath, DESTINATIONS[1]);
    initPath(paths.videoPath, DESTINATIONS[2]);
    initPath(paths.picPath, DESTINATIONS[3]);
    initPath(paths.docPath, DESTINATIONS[4]);
    initDaemon();
    while (1)
    {
        sleep(5);
        logger(INFO, paths.downPath);
    }
    close_log();
    return 0;
}
