#include <stdio.h>
#include <stdlib.h>
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
char downPath[PATH_LENGTH] = "/home/";
char musicPath[PATH_LENGTH] = "/home/";
char videoPath[PATH_LENGTH] = "/home/";
char picPath[PATH_LENGTH] = "/home/";
char docPath[PATH_LENGTH] = "/home/";
int main(void)
{
    initPath(downPath,DESTINATIONS[0]);
    initPath(musicPath,DESTINATIONS[1]);
    initPath(videoPath,DESTINATIONS[2]);
    initPath(picPath,DESTINATIONS[3]);
    initPath(docPath,DESTINATIONS[4]);
    printf("Downloads destination: %s \n", downPath);
    printf("Music destination: %s \n", musicPath);
    return 0;
}
