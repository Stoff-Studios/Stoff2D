/* 
 *
 * clm.h
 *
 * Tailored for OpenGL projects using C.
 *
 * Written by Jesse Fryer
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float x;
    float y;
} clmVec2;

typedef struct {
    float x;
    float y;
    float z;
} clmVec3;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} clmVec4;

typedef struct {
    float mat[16];
} clmMat4;

#define CLM_PI 3.141592653589793238

/* clm_radians
 * -----------
 * Convert degrees to radians.
 */
float clm_radians(float deg);

/* clm_v2_dot_product
 * ------------------
 * Returns the dot product of v1 and v2.
 */
float clm_v2_dot(clmVec2 v1, clmVec2 v2);

/* clm_v3_dot_product
 * ------------------
 * Returns the dot product of v1 and v2.
 */
float clm_v3_dot_product(clmVec3 v1, clmVec3 v2);

/* clm_v2_add
 * ----------
 * Returns a new clmVec2 which is v1 + v2
 */
clmVec2 clm_v2_add(clmVec2 v1, clmVec2 v2);

/* clm_v3_add
 * ----------
 * Returns a new clmVec3 which is v1 + v2
 */
clmVec3 clm_v3_add(clmVec3 v1, clmVec3 v2);

/* clm_v2_scalar_mul
 * -----------------
 * Returns a new clmVec2 which is scalar * v
 */
clmVec2 clm_v2_scalar_mul(float scalar, clmVec2 v);

/* clm_v3_scalar_multiply
 * ----------------------
 * Returns a new clmVec3 which is scalar * v
 */
clmVec3 clm_v3_scalar_mul(float scalar, clmVec3 v);

/* clm_v2_length
 * -------------
 * Returns the length of vector v.
 */
float clm_v2_len(clmVec2 v);

/* clm_v3_length
 * -------------
 * Returns the length of vector v.
 */
float clm_v3_len(clmVec3 v);

/* clm_v2_normalize
 * ----------------
 * Return normalized v.
 */
clmVec2 clm_v2_normalize(clmVec2 v);

/* clm_v3_normalize
 * ----------------
 * Return normalized v.
 */
clmVec3 clm_v3_normalize(clmVec3 v);

/* clm_v3_cross_product
 * --------------------
 * Returns a new clmVec3 which is the cross product 
 * of v1 and v2.
 */
clmVec3 clm_v3_cross(clmVec3 v1, clmVec3 v2);

/* clm_mat4_mul
 * ------------
 * Compute  mat1 x mat2, stores result in mat1.
 */
clmMat4 clm_mat4_mul_mat4(clmMat4 mat1, clmMat4 mat2);

/* clm_mat4_multiply_v4
 * --------------------
 * Returns new clmVec4 which is mat x v
 */
clmVec4 clm_mat4_mul_v4(clmMat4 mat, clmVec4 v);

/* clm_mat4_identity
 * -----------------
 * Returns a new clmMat4 which is the identity matrix.
 */
clmMat4 clm_mat4_identity();

/* clm_mat4_translate
 * ------------------
 * Returns new mat4 which is transform with translation 
 * added to it.
 */
clmMat4 clm_mat4_translate(
        clmMat4 transform, 
        clmVec3 translation);

/* clm_mat4_rotate
 * ---------------
 * Returns a new clmMat4 with a rotation to trans along axis. 
 */
clmMat4 clm_mat4_rotate(
        clmMat4 trans,
        float   degrees,
        clmVec3 axis);

/* clm_mat4_scale
 * --------------
 * Returns a new clmMat4 which is trans scaled by scale.
 */
clmMat4 clm_mat4_scale(clmMat4 trans, clmVec3 scale);

/* clm_mat4_ortho
 * --------------
 * Returns a new clmMat4 which is an orthographic projection matrix.
 */
clmMat4 clm_mat4_ortho(
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far);

/* clm_mat4_perspective
 * --------------------
 * Returns a new clmMat4 which is a perspective projection matrix.
 */
clmMat4 clm_mat4_perspective(
        float fov, 
        float aspectRatio, 
        float near, 
        float far);

/* clm_mat4_lookat
 * ---------------
 * Returns a new clmMat4 which is the lookat matrix.
 */
clmMat4 clm_mat4_lookat(
        clmVec3 position,
        clmVec3 target,
        clmVec3 up);

/* clm_mat4_print
 * --------------
 * print mat to 2 decimals points accuracy
 */
void clm_mat4_print(clmMat4 mat);

/* clm_vec2_print
 * --------------
 * print v to 4 decimals points accuracy
 */
void clm_v2_print(clmVec2 v);

/* clm_vec3_print
 * --------------
 * print v to 4 decimals points accuracy
 */
void clm_v3_print(clmVec3 v);

/* clm_vec4_print
 * --------------
 * print v to 4 decimals points accuracy
 */
void clm_v4_print(clmVec4 v);

#ifdef __cplusplus
}
#endif
