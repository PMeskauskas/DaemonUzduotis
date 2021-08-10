#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <libconfig.h>
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
static config_t cfg;
static config_setting_t *setting = NULL;
const char *str = NULL;

static void configCheck();
static void configWatchPath(const char *str);
static void configWatchTypes(int *audio, int *video, int *photo, int *document);
static void searchFile();
static void watchDir(char fileName[]);
static int findFileType(char fileName[]);
static int checkType(char *path, const char *str, char *type);
static void checkFile(int index, char fileName[]);
static void moveTo(char oldpath[], char newpath[]);
int system(const char *command);

int main(void)
{
    open_log();
    
    config_init(&cfg);
    configCheck();
    configWatchPath(str);
    pathInit(paths.musicPath, DESTINATIONS[1]);
    pathInit(paths.videoPath, DESTINATIONS[2]);
    pathInit(paths.picPath, DESTINATIONS[3]);
    pathInit(paths.docPath, DESTINATIONS[4]);
    initDaemon(process_id);
    searchFile();
    config_destroy(&cfg);
    close_log();
    return 0;
}

static void configCheck()
{
    if (!config_read_file(&cfg, "configure.cfg"))
    {
        logger(ERROR, "Failed to read config file");
        config_destroy(&cfg);
        exit(1);
    }
}

static void configWatchPath(const char *str)
{
    if (config_lookup_string(&cfg, "dir_to_watch", &str))
    {
        logger(INFO, "dir_to_watch found.");
        if (strlen(str) > 0)
        {
            strcpy(paths.downPath, str);
            pathExists(paths.downPath);
        }
        else
        {
            logger(WARNING, "No dir_to_watch specified, using default directory...");
            pathInit(paths.downPath, DESTINATIONS[0]);
        }
    }
    else
    {
        logger(WARNING, "No 'dir_to_watch' setting in configuration file, using default...");
        pathInit(paths.downPath, DESTINATIONS[0]);
    }
}
static void configWatchTypes(int *audio, int *video, int *photo, int *document)
{
    setting = config_lookup(&cfg, "types_to_watch");
    if (setting != NULL)
    {
        config_setting_t *types = config_setting_get_elem(setting, 0);
        int audiotype, videotype, phototype, documenttype;
        if (!(config_setting_lookup_int(types, "audio", &audiotype) && config_setting_lookup_int(types, "video", &videotype) && config_setting_lookup_int(types, "photo", &phototype) && config_setting_lookup_int(types, "document", &documenttype)))
        {
            logger(ERROR, "Failed to find types_to_watch types in .cfg file, shutting down... ");
            kill(process_id, SIGKILL);
        }

        *audio = audiotype;
        *video = videotype;
        *photo = phototype;
        *document = documenttype;
    }
    else
    {
        logger(ERROR, "No types_to_watch specified, shutting down...");
        kill(process_id, SIGKILL);
    }
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
            checkFile(i, dir->d_name);
        }
    }
    closedir(dp);
}
static int findFileType(char fileName[])
{
    char *type = strrchr(fileName, '.');
    const char *str = NULL;
    if (type != NULL)
    {
        int audio = 0, video = 0, photo = 0, document = 0;
        configWatchTypes(&audio, &video, &photo, &document);
        if (audio == 0)
        {
            if (checkType("audio_types", str, type) == 0)
                return MUSIC;
        }
        if (video == 0)
        {
            if (checkType("video_types", str, type) == 0)
                return VIDEOS;
        }
        if (photo == 0)
        {
            if (checkType("photo_types", str, type) == 0)
                return PICTURES;
        }
        if (document == 0)
        {
            if (checkType("document_types", str, type) == 0)
                return DOCUMENTS;
        }
    }
    return -1;
}
static int checkType(char *path, const char *str, char *type)
{
    if (config_lookup_string(&cfg, path, &str))
    {
        char *string = (char *)malloc(sizeof(char) * 10);

        strcpy(string, str);
        char *token = strtok(string, ",");
        while (token != NULL)
        {
            if (strcmp(type, token) == 0)
            {
                free(string);
                return 0;
            }
            token = strtok(NULL, ",");
        }
        free(string);
        return -1;
    }
    else
        return -1;
}
static void checkFile(int index, char fileName[])
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
    
    if(pathCheck(newpath, oldpath, process_id) == 0){

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
}
