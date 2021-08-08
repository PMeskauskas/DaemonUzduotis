#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include "path.h"
#include "logger.h"
#include "daemon.h"

#define PATH_LENGTH 256

struct path
{
    char downPath[PATH_LENGTH];
    char musicPath[PATH_LENGTH];
    char videoPath[PATH_LENGTH];
    char picPath[PATH_LENGTH];
    char docPath[PATH_LENGTH];
};

enum LOCATION
{
    DOWNLOADS,
    MUSIC,
    VIDEOS,
    PICTURES,
    DOCUMENTS
};

static char *DESTINATIONS[] = {
    "/Downloads", "/Music", "/Videos", "/Pictures", "/Documents"};

static struct path paths = {
    "/home/", "/home/", "/home/", "/home/", "/home/"};

static pid_t process_id = 0;

static void searchFile();
static int findFileType(char fileName[]);
static void moveFile(int index, char fileName[]);
static void moveTo(char oldpath[], char newpath[]);

int main(void)
{
    open_log();
    pathInit(paths.downPath, DESTINATIONS[0]);
    pathInit(paths.musicPath, DESTINATIONS[1]);
    pathInit(paths.videoPath, DESTINATIONS[2]);
    pathInit(paths.picPath, DESTINATIONS[3]);
    pathInit(paths.docPath, DESTINATIONS[4]);
    //initDaemon(process_id);
    searchFile();
    close_log();
    return 0;
}

static void searchFile()
{
    DIR *dp;
    struct dirent *dir;
    if ((dp = opendir(paths.downPath)) == NULL)
    {
        kill(process_id, SIGKILL);
    }
    sleep(5);
    while ((dir = readdir(dp)) != NULL)
    {
        if (dir->d_name[0] != '.')
        {
            int i = findFileType(dir->d_name);
            printf("File: %s, filetype: %d \n", dir->d_name, i);
            moveFile(i, dir->d_name);
        }
    }

    closedir(dp);
}

static int findFileType(char fileName[])
{
    char *type = strrchr(fileName, '.');
    if (type != NULL)
    {
        if (strcmp(type, ".mp3") == 0 || strcmp(type, ".wav") == 0 || strcmp(type, ".flac") == 0)
        {
            return MUSIC;
        }
        else if (strcmp(type, ".mkv") == 0 || strcmp(type, ".avi") == 0 || strcmp(type, ".mp4") == 0)
        {
            return VIDEOS;
        }
        else if (strcmp(type, ".jpeg") == 0 || strcmp(type, ".png") == 0 || strcmp(type, ".svg") == 0)
        {
            return PICTURES;
        }
        else if (strcmp(type, ".txt") == 0 || strcmp(type, ".docx") == 0 || strcmp(type, ".pdf") == 0)
        {
            return DOCUMENTS;
        }
    }
    return -1;
}

static void moveFile(int index, char fileName[])
{
    char filePath[PATH_LENGTH];
    strcpy(filePath, paths.downPath);
    strcat(filePath, "/");
    strcat(filePath, fileName);
    printf("old path: %s\n", filePath);
    printf("old path: %s\n", paths.musicPath); 
    int rc = rename(filePath, paths.musicPath);
    printf("perjungimas: %d\n",rc);
    switch (index)
    {
    case (MUSIC):
        moveTo(filePath, paths.musicPath);
        break;
    case (VIDEOS):
        moveTo(filePath, paths.musicPath);
        break;
    case (PICTURES):
        moveTo(filePath, paths.musicPath);
        break;
    case (DOCUMENTS):
        moveTo(filePath, paths.musicPath);
        break;
    default:
        moveTo(filePath, paths.musicPath);
    }
    
}

static void moveTo(char oldpath[], char newpath[])
{
    if (rename(oldpath, newpath))
    {
        logger(WARNING, "Couldn't move file, Deleting...");
        remove(oldpath);
    }
}
