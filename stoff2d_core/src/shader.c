#include <glad/glad.h>

#include <shader.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SHADER_FOLDER S2D_SHADER_FOLDER

// Helper method which reads a text file into a string.
char* read_shader_file(const char* filePath) {
    // Open the homie.
    FILE* srcFile = fopen(filePath, "r");

    // Check for errors.
    if (!srcFile) {
        fprintf(stderr,
                "[S2D Error] couldn't open shader file %s\n",
                filePath);
        return NULL;
    }

    // Get the file size.
    fseek(srcFile, 0L, SEEK_END);
    int fileSize = ftell(srcFile);
    rewind(srcFile);

    // Allocate memory and read src file.
    char* src = (char*) malloc((fileSize * sizeof(char)) + 1);
    size_t srcIndex = 0;
    char tok;
    while ((tok = fgetc(srcFile)) != EOF) {
        src[srcIndex++] = tok;
    }
    src[srcIndex] = '\0';

    fclose(srcFile);

    return src;
}

unsigned int s2d_shader_create(
        const char* vShaderPath,
        const char* fShaderPath) {
    // Shader directory path.
    const char* shaderDir = SHADER_FOLDER;

    // Construct path to vertex shader.
    size_t vPathLen = strlen(shaderDir) + strlen(vShaderPath);
    char* vPath = (char*) malloc((vPathLen * sizeof(char)) + 1);
    strcpy(vPath, shaderDir);
    strcat(vPath, vShaderPath);

    // Construct path to fragment shader.
    size_t fPathLen = strlen(shaderDir) + strlen(fShaderPath);
    char* fPath = (char*) malloc((fPathLen * sizeof(char)) + 1);
    strcpy(fPath, shaderDir);
    strcat(fPath, fShaderPath);

    // Read the src code into strings.
    char* vShaderSrc = read_shader_file(vPath);
    char* fShaderSrc = read_shader_file(fPath);

    free(vPath);
    free(fPath);

    unsigned int vShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    // fucked pointer cast fixes Werror=incompatible-pointer-types
    glShaderSource(vShader, 1, (const GLchar * const*) &vShaderSrc, NULL);
    free(vShaderSrc);
    glCompileShader(vShader);

    unsigned int fShader;
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    // fucked pointer cast fixes Werror=incompatible-pointer-types
    glShaderSource(fShader, 1, (const GLchar * const*) &fShaderSrc, NULL);
    free(fShaderSrc);
    glCompileShader(fShader);

    unsigned int shader;
    shader = glCreateProgram();
    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return shader;
}

void s2d_shader_use(unsigned int shader) {
    glUseProgram(shader);
}

void s2d_shader_set_uniform_mat4(
        unsigned int shader,
        const char* uniformName,
        clmMat4 mat) {
    s2d_shader_use(shader);
    unsigned int loc = glGetUniformLocation(shader, uniformName);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat.mat);
}

void s2d_shader_set_uniform_vec3(
        unsigned int shader,
        const char* uniformName,
        clmVec3 vec) {
    s2d_shader_use(shader);
    unsigned int loc = glGetUniformLocation(shader, uniformName);
    glUniform3fv(loc, 1, &vec.x);
}

void s2d_shader_set_uniform_1i(
        unsigned int shader,
        const char* uniformName,
        i32 i) {
    s2d_shader_use(shader);
    unsigned int loc = glGetUniformLocation(shader, uniformName);
    glUniform1i(loc, i);
}

void s2d_shader_set_uniform_1f(
        unsigned int shader,
        const char* uniformName,
        float f) {
    s2d_shader_use(shader);
    unsigned int loc = glGetUniformLocation(shader, uniformName);
    glUniform1f(loc, f);
}

void s2d_shader_set_sampler2d(
        unsigned int shader, 
        const char* uniformName,
        u32 texID,
        u32 slot) {
    s2d_shader_use(shader);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texID);
    unsigned int loc = glGetUniformLocation(shader, uniformName);
    glUniform1i(loc, slot);
}
