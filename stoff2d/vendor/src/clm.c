#include <clm/clm.h>
#include <math.h>
#include <stdio.h>

float clm_radians(float deg) {
    return deg * (CLM_PI / 180.0f);
}

float clm_v2_dot(clmVec2 v1, clmVec2 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}

float clm_v3_dot(clmVec3 v1, clmVec3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

clmVec2 clm_v2_add(clmVec2 v1, clmVec2 v2) {
    return (clmVec2) {
        .x = v1.x + v2.x, 
        .y = v1.y + v2.y
    };
}

clmVec3 clm_v3_add(clmVec3 v1, clmVec3 v2) {
    return (clmVec3) {
        .x = v1.x + v2.x, 
        .y = v1.y + v2.y,
        .z = v1.z + v2.z
    };
}

clmVec2 clm_v2_scalar_mul(float scalar, clmVec2 v) {
    return (clmVec2) {
        .x = scalar * v.x,
        .y = scalar * v.y
    };
}

clmVec3 clm_v3_scalar_mul(float scalar, clmVec3 v) {
    return (clmVec3) {
        .x = scalar * v.x,
        .y = scalar * v.y,
        .z = scalar * v.z
    };
}

float clm_v2_len(clmVec2 v) {
    return sqrtf((v.x * v.x) + (v.y * v.y));
}

float clm_v3_len(clmVec3 v) {
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

clmVec2 clm_v2_normalize(clmVec2 v) {
    float len = clm_v2_len(v);
    if (len) {
        float invLen = 1.0f / len;
        return clm_v2_scalar_mul(invLen, v);
    }
    return v;
}

clmVec3 clm_v3_normalize(clmVec3 v) {
    float len = clm_v3_len(v);
    if (len) {
        float invLen = 1.0f / len;
        return clm_v3_scalar_mul(invLen, v);
    }
    return v;
}

clmVec3 clm_v3_cross(clmVec3 v1, clmVec3 v2) {
    return (clmVec3) {
        .x = (v1.y * v2.z) - (v1.z * v2.y),
        .y = (v1.z * v2.x) - (v1.x * v2.z),
        .z = (v1.x * v2.y) - (v1.y * v2.x)
    };
}

clmMat4 clm_mat4_mul_mat4(clmMat4 mat1, clmMat4 mat2) {
    // Compute the matrix multiplication into tmp.
    clmMat4 tmp;
    for (int i = 0; i < 16; i++) {
        int mat1Idx = i % 4;
        int mat2Idx = (i / 4) * 4;
        tmp.mat[i] = 
            (mat1.mat[mat1Idx + 0]  * mat2.mat[mat2Idx + 0]) +
            (mat1.mat[mat1Idx + 4]  * mat2.mat[mat2Idx + 1]) +
            (mat1.mat[mat1Idx + 8]  * mat2.mat[mat2Idx + 2]) +
            (mat1.mat[mat1Idx + 12] * mat2.mat[mat2Idx + 3]);
    }

    return tmp;
}

clmVec4 clm_mat4_mul_v4(clmMat4 mat, clmVec4 v) {
    clmVec4 tmp;

    for  (int i = 0; i < 4; i++) {
        float* element;
        switch (i) {
            case 0:
                element = &tmp.r;
                break;
            case 1:
                element = &tmp.g;
                break;
            case 2:
                element = &tmp.r;
                break;
            case 3:
                element = &tmp.a;
                break;
        }

        *element =
            (mat.mat[0  + i]  * v.r) + 
            (mat.mat[4  + i]  * v.g) +
            (mat.mat[8  + i]  * v.b) +
            (mat.mat[12 + i]  * v.a);
    }

    return tmp;
}

clmMat4 clm_mat4_identity() {
    return (clmMat4) {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

clmMat4 clm_mat4_translate(
        clmMat4 transform,
        clmVec3 translation) {
    clmMat4 translateMat = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation.x, translation.y, translation.z, 1.0f
    };

    return clm_mat4_mul_mat4(transform, translateMat);
}

clmMat4 clm_mat4_rotate(
        clmMat4 trans,
        float rad, 
        clmVec3 axis) {
    float rotX = axis.x;
    float rotY = axis.y;
    float rotZ = axis.z;
    float sinTheta = sinf(rad);
    float cosTheta = cosf(rad);
    float justCoz = 1 - cosTheta; // This is reused a lot.

    clmMat4 rotMat = {
        cosTheta + (rotX * rotX * justCoz),
        (rotY * rotX * justCoz) + (rotZ * sinTheta),
        (rotZ * rotX * justCoz) - (rotY * sinTheta),
        0,

        (rotX * rotY * justCoz) - (rotZ * sinTheta),
        cosTheta + (rotY * rotY * justCoz),
        (rotZ * rotY * justCoz) + (rotX * sinTheta),
        0,

        (rotX * rotZ * justCoz) + (rotY * sinTheta),
        (rotY * rotZ * justCoz) - (rotX * sinTheta),
        cosTheta + (rotZ * rotZ * justCoz),
        0,

        0,
        0,
        0,
        1
    };

    return clm_mat4_mul_mat4(trans, rotMat);
}

clmMat4 clm_mat4_scale(clmMat4 trans, clmVec3 scale) {
    clmMat4 scaleMat = {
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1
    };

    return clm_mat4_mul_mat4(trans, scaleMat);
}

clmMat4 clm_mat4_ortho(
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far) {
    // Initialise new matrix as null matrix.
    clmMat4 proj;
    for (int i = 0; i < 16; i++) {
        proj.mat[i] = 0.0f;
    }

    // Put bits in there.
    proj.mat[0]  =  2.0f / (right - left);
    proj.mat[5]  =  2.0f / (top - bottom);
    proj.mat[10] = -2.0f / (far - near);
    proj.mat[12] = (-1 * (right + left)) / (right - left);
    proj.mat[13] = (-1 * (top + bottom)) / (top - bottom);
    proj.mat[14] = (-1 * (far + near)) / (far - near);
    proj.mat[15] = 1.0f;

    return proj;
}

clmMat4 clm_mat4_perspective( 
        float fov, 
        float aspectRatio, 
        float near, 
        float far) {
    // Yanked this from:
    //     Learn WebGL 8.3 Perspective Projections.

    // We need left, right, top and bottom of frustum.
    float top = near * tanf(fov * 0.5);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;
    
    // Now lets a go.
    // 0 it all out first.
    clmMat4 proj;
    for (int i = 0; i < 16; i++) {
        proj.mat[i] = 0.0f;
    }

    // Place all the bits in there.
    proj.mat[0]  = (2.0f * near) / (right - left);
    proj.mat[5]  = (2.0f * near) / (top - bottom);
    proj.mat[10] = (-(far + near)) / (far - near);
    proj.mat[11] = -1.0f;
    proj.mat[12] = (-near * (right + left)) / (right - left);
    proj.mat[13] = (-near * (top + bottom)) / (top - bottom);
    proj.mat[14] = (2.0f * far * near) / (near - far);

    return proj;
}

clmMat4 clm_mat4_lookat(
        clmVec3 position,
        clmVec3 target,
        clmVec3 up) {
    // Get the camera face vector.
    clmVec3 camDir = clm_v3_add(
            position, 
            clm_v3_scalar_mul(-1.0f, target));
    camDir = clm_v3_normalize(camDir);

    // Get the right vector.
    clmVec3 camRight = clm_v3_cross(up, camDir);
    camRight = clm_v3_normalize(camRight);

    // Get the camera up vector.
    clmVec3 camUp = clm_v3_cross(camDir, camRight);

    // Multiply these two together after setting them up.
    clmMat4 positionTrans = clm_mat4_identity();
    clmMat4 lookat = clm_mat4_identity();

    lookat.mat[0] = camRight.x;
    lookat.mat[4] = camRight.y;
    lookat.mat[8] = camRight.z;

    lookat.mat[1] = camUp.x;
    lookat.mat[5] = camUp.y;
    lookat.mat[9] = camUp.z;

    lookat.mat[2]  = camDir.x;
    lookat.mat[6]  = camDir.y;
    lookat.mat[10] = camDir.z;

    positionTrans.mat[12] = -position.x;
    positionTrans.mat[13] = -position.y;
    positionTrans.mat[14] = -position.z;

    return clm_mat4_mul_mat4(lookat, positionTrans);
}

void clm_mat4_print(clmMat4 mat) {
    for (int i = 0; i < 4; i++) {
        // Print each row.
        for (int j = 0; j < 4; j++) {
            printf("%.2f ", mat.mat[i + (j * 4)]);
        }
        printf("\n");
    }
}

void clm_v2_print(clmVec2 v) {
    printf("(%.4f, %.4f)", v.x, v.y);
}

void clm_v3_print(clmVec3 v) {
    printf("(%.4f, %.4f, %.4f)", v.x, v.y, v.z);
}

void clm_v4_print(clmVec4 v) {
    printf("(%.4f, %.4f, %.4f, %.4f)",
            v.r, v.g, v.b, v.a);
}
