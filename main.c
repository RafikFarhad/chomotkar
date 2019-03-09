#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOCATION "http://picsum.photos/3840/2400/?random"
#define SAVING_PATH ".chomotkar"
#define VERSION "0.13"
void GetFileName(char *name);
int download_url(char const *const url, char *fileName);
void PreWork();
int SetWallpaper(char *fullPath);

int main(int arg_c, char arg_v[])
{
    printf(".:: Chomotkar - %s ::.\n", VERSION);
    if(arg_c==2)
    {
        printf("\n\nChomotkar:\n\nVersion: %s does not support any flags.\n", VERSION);
        return 0;
    }
    PreWork();
    char outFileName[4096];
    GetFileName(outFileName);
    int result = download_url(LOCATION, outFileName);
    if(result <= 0)
    {
        printf("Sorry an error has occured while downloading wallpaper.\n");
    }
    else 
    {
        printf("New wallpaper downloaded.\n");
        printf("Size: %d KB\n", result/1024);
        printf("Path: %s\n", outFileName);
        int result = SetWallpaper(outFileName);
        if(result < 0)
        {
            printf("Something wen wrong while setting up wallpaper.\nPlease inform the developer <rafikfarhad@gmail.com> to fix this bug.\n");
        }
        else
        {
            printf("Enjoy Your New Home Screen!\n");
        }
    }
}

void GetFileName(char *name)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char const *const pre = getenv("HOME");
    sprintf(name, "%s/%s/wallpaper_%d_%d_%d_%d_%d_%d.jpeg", pre, SAVING_PATH, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void PreWork()
{
    char makeDirectoryCommand[1024];
    char const *const homeDirectory = getenv("HOME");
    sprintf(makeDirectoryCommand, "mkdir -p %s/%s", homeDirectory, SAVING_PATH);
    FILE *fp = popen(makeDirectoryCommand, "r");
    if(fp == NULL)
    {
        printf("Could not create folder on ~\n");
    }
}

int SetWallpaper(char *fullPath)
{
    char changeWallpaperCommand[1024];
    sprintf(changeWallpaperCommand, "gsettings set org.gnome.desktop.background picture-uri %s", fullPath);
    FILE *fp = popen(changeWallpaperCommand, "r");
    if(fp == NULL)
    {
        return -1;
    }
    return 0;
}

// gcc -o chomotkar main.c downloadHelper.c -lcurl && ./chomotkar
// http://simmesimme.github.io/lessons/2011/11/02/ppa-launchpad-cmake
// echo $XDG_CURRENT_DESKTOP