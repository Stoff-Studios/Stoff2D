#include <utils.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h> 
#include <unistd.h>
#endif

#ifdef _WIN32
char** list_files_in_dir(const char* dirPath) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    char** files = malloc(sizeof(char*));

    char sPath[2048];
    sprintf(sPath, "%s\\*.*", dirPath);
    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        fprintf(stderr, 
                "[S2D Error] could not open directory - %s\n", 
                dirPath); 
        return NULL; 
    }

    size_t index = 0;
    do {
        if (!strcmp(fdFile.cFileName, ".") 
                || !strcmp(fdFile.cFileName, "..")) {
            continue;
        }
        files[index++] = strdup(fdFile.cFileName);
        files = realloc(files, sizeof(char*) * (index + 1));
    } while (FindNextFile(hFind, &fdFile));
    files[index] = NULL;

    FindClose(hFind);

    return files;
}
#else

char** list_files_in_dir(const char* dirPath) {
    char** files = malloc(sizeof(char*));
    DIR*   dir;

    if (!(dir = opendir(dirPath))) { 
        fprintf(stderr, 
                "[S2D Error] could not open directory - %s\n", 
                dirPath); 
        return NULL; 
    }

    size_t index = 0;
    struct dirent* dirEntry;
    while ((dirEntry = readdir(dir))) {
        files[index++] = strdup(dirEntry->d_name);
        files = realloc(files, sizeof(char*) * (index + 1));
    }
    files[index] = NULL;

    closedir(dir);

    return files;
}

#endif


void utils_sleep(float seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000.0f);
#else
    usleep(seconds * 1000000);
#endif
}
