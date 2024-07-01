#pragma once

#include <clm/clm.h>
#include <settings.h>
#include <defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/* s2d_shader_create
 * -----------------
 * Returns id handle of shader program.
 */
unsigned int s2d_shader_create(
        const char* vShaderPath,
        const char* fShaderPath);

/* s2d_shader_use
 * --------------
 * Sets the given shader program handle as the currently used
 * shader.
 */
void s2d_shader_use(unsigned int shader);

/* s2d_shader_set_uniform_mat4
 * ---------------------------
 * Set a mat4 uniform in shader.
 */
void s2d_shader_set_uniform_mat4(
        unsigned int shader,
        const char* uniformName,
        clmMat4 mat);

/* s2d_shader_set_uniform_vec3
 * ---------------------------
 * Set a vec3 uniform in shader.
 */
void s2d_shader_set_uniform_vec3(
        unsigned int shader,
        const char* uniformName,
        clmVec3 vec);

/* s2d_shader_set_uniform_1i
 * -------------------------
 * Set an Integer uniform in shader.
 */
void s2d_shader_set_uniform_1i(
        unsigned int shader,
        const char* uniformName,
        i32 i);

/* s2d_shader_set_uniform_1f
 * -------------------------
 * Set a float uniform in shader.
 */
void s2d_shader_set_uniform_1f(
        unsigned int shader,
        const char* uniformName,
        float f);

#ifdef __cplusplus
}
#endif
