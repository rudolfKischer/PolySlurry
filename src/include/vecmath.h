#ifndef VECMATH_H
#define VECMATH_H
#include <math.h>
// vector

#define PI 3.14159265358979323846


typedef union {
    struct {
        float x, y, z;
    };
    float v[3];
} vec3;

typedef union {
    struct {
        float x, y, z, w;
    };
    float v[4];
} vec4;

typedef struct {
    float v[4][4];
} mat4;



// vec3 function declarations
vec3 zero3();
vec3 a3(vec3 a, vec3 b);
vec3 p3(vec3 a, vec3 b);
vec3 s3(vec3 a, float b);
vec3 n3(vec3 a);
vec3 i3(vec3 a);
vec3 min3(vec3 a, vec3 b);
vec3 max3(vec3 a, vec3 b);
float mini3(vec3 a);
float maxi3(vec3 a);
float d3(vec3 a, vec3 b);
float l3(vec3 a);
vec3 normalize3(vec3 a);
void printVec3(vec3 a);

// vec4 function declarations
vec4 zero4();
vec4 a4(vec4 a, vec4 b);
vec4 p4(vec4 a, vec4 b);
vec4 s4(vec4 a, float b);
vec4 n4(vec4 a);
vec4 i4(vec4 a);
float d4(vec4 a, vec4 b);
float l4(vec4 a);
vec4 normalize4(vec4 a);

vec4 v3_to_v4(vec3 a);
vec3 v4_to_v3(vec4 a);
vec4 hmg_coord(vec3 a);
vec3 unhmg_coord(vec4 a);
mat4 dm4(mat4 A, mat4 B);
vec4 dm4v4(mat4 A, vec4 b);
vec3 apply(mat4 transform, vec3 v);
mat4 skewm4(vec3 a);
vec3 x3(vec3 a, vec3 b);
mat4 sm4(mat4 A, float s);
mat4 am4(mat4 A, mat4 B);
mat4 t4(mat4 a);
mat4 Im4();
mat4 uscale(float a);
mat4 scale(vec3 a);
mat4 translate(vec3 a);
mat4 rotate(vec3 u, float theta);
mat4 rotateEuler(vec3 eulerAngles);
mat4 orthographic(vec3 minp, vec3 maxp);
mat4 perspective(vec3 minp, vec3 maxp);
mat4 viewFrame(vec3 eye, vec3 target, vec3 up);
mat4 view(vec3 eye, vec3 target, vec3 up);
mat4 ndc();

#endif 
