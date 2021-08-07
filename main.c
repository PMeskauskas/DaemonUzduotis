#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include "path.h"
#include "logger.h"
#include "daemon.h"
#include "search.h"
#define PATH_LENGTH 40
struct path {
    char downPath[PATH_LENGTH];
    char musicPath[PATH_LENGTH];
    char videoPath[PATH_LENGTH];
    char picPath[PATH_LENGTH];
    char docPath[PATH_LENGTH];
};
typedef enum LOCATION {
    DOWNLOADS ,MUSIC, VIDEOS, PICTURES, DOCUMENTS
} LOCATION;
static char *DESTINATIONS[] = {
    "/Downloads","/Music", "/Videos", "/Pictures", "/Documents"
};
static struct path paths = {
        "/home/", "/home/", "/home/", "/home/","/home/"  
        };
static pid_t process_id = 0;
static void searchFile();
static int findFileType();
int main(void)
{
    open_log();
    pathInit(paths.downPath, DESTINATIONS[0]);
    pathInit(paths.musicPath, DESTINATIONS[1]);
    pathInit(paths.videoPath, DESTINATIONS[2]);
    pathInit(paths.picPath, DESTINATIONS[3]);
    pathInit(paths.docPath, DESTINATIONS[4]);
    initDaemon(process_id);
    searchFile();
    close_log();
    return 0;
}
static void searchFile()
{
    DIR *dp;
    struct dirent *dir;
    if((dp = opendir(paths.downPath)) == NULL){
        logger(ERROR, "Failed to open directory, shutting down...");
        kill(process_id, SIGKILL);
    }
    while (1)
    {
        sleep(5);
        while((dir = readdir(dp)) != NULL) {
            printf(dir->d_name);
        }
        
    }
}
static int findFileType()
{
    
}