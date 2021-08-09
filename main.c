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
static void watchDir(char fileName[]);
static int findFileType(char fileName[]);
static void moveFile(int index, char fileName[]);
static void moveTo(char oldpath[], char newpath[]);
int system(const char *command);

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
    while (1)
    {
        sleep(5);
        watchDir(paths.downPath);
    }
}

static void watchDir(char fileName[])
{
    struct dirent *dir = NULL;
    DIR *dp = NULL;
    if ((dp = opendir(fileName)) == NULL)
    {
        logger(ERROR, "Failed to open directory, shutting down");
        kill(process_id, SIGKILL);
    }
    while ((dir = readdir(dp)) != NULL)
    {
        if (dir->d_name[0] != '.')
        {
            int i = findFileType(dir->d_name);
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
    switch (index)
    {
    case (MUSIC):
        moveTo(filePath, paths.musicPath);
        break;
    case (VIDEOS):
        moveTo(filePath, paths.videoPath);
        break;
    case (PICTURES):
        moveTo(filePath, paths.picPath);
        break;
    case (DOCUMENTS):
        moveTo(filePath, paths.docPath);
        break;
    default:
        break;
    }
}

static void moveTo(char oldpath[], char newpath[])
{
    char msg[256];
    char move[256];

    pathCheck(newpath, process_id);

    strcpy(msg, "File ");
    strcat(msg, oldpath);

    strcpy(move, "mv ");
    strcat(move, oldpath);
    strcat(move, " ");
    strcat(move, newpath);

    if (system(move) != -1)
    {
        strcat(msg, " moved successfully");
        logger(INFO, msg);
    }
    else
    {
        strcat(msg, " failed to move, deleting...");
        logger(WARNING, msg);
        remove(oldpath);
    }
}
