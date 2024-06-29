#include <defines.h>
#include <glad/glad.h>

#include <utils.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RENDER_TEX_W 1024
#define RENDER_TEX_H 1024

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

u32 new_rendertexture_framebuffer(u32 w, u32 h) {
    // framebuffer.
    u32 frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    // render texture.
    u32 renderedTexture;
    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            w,
            h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glFramebufferTexture(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            renderedTexture,
            0);
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "[S2D Error] failed to create font render texture\n");
    }

    glViewport(0, 0, w, h);
    // 0 out texture data.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    return renderedTexture;
}
