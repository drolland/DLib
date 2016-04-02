#pragma once

typedef float mat4[4][4];
typedef float vec3[3];

void d_mat4_mul_mat4(mat4 res,mat4 a,mat4 b);

void d_mat4_mul_vec3(vec3 res, mat4 m, vec3);

void d_mat4_perspective(mat4 res, float fovy, float apect_ratio, float near, float far);

void d_mat4_print(mat4 mat);

