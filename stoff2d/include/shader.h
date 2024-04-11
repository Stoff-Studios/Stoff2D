#pragma once

#include <clm/clm.h>
#include <settings.h>
#include <defines.h>

#define SHADER_FOLDER S2D_SHADER_FOLDER

/* shader_create
 * -------------
 * Returns id handle of shader program.
 */
unsigned int shader_create(
        const char* vShaderPath,
        const char* fShaderPath);

/* shader_use
 * ----------
 * Sets the given shader program handle as the currently used
 * shader.
 */
void shader_use(unsigned int shader);

/* shader_set_uniform_mat4
 * -----------------------
 * Set a mat4 uniform in shader.
 */
void shader_set_uniform_mat4(
        unsigned int shader,
        const char* uniformName,
        clmMat4 mat);

/* shader_set_uniform_vec3
 * -----------------------
 * Set a vec3 uniform in shader.
 */
void shader_set_uniform_vec3(
        unsigned int shader,
        const char* uniformName,
        clmVec3 vec);

/* shader_set_uniform_1i
 * ---------------------
 * Set an Integer uniform in shader.
 */
void shader_set_uniform_1i(
        unsigned int shader,
        const char* uniformName,
        i32 i);
