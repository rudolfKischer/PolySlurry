#ifndef CAMERA_H
#define CAMERA_H
#include "vecmath.h"

typedef enum {
    PERSPECTIVE,
    ORTHOGRAPHIC,
} PROJECTIONTYPE;


typedef struct Camera {
    vec3 position;
    vec3 viewDir;
    vec3 up;
    float fov;
    float aspect;
    float near;
    float far;
    PROJECTIONTYPE projection;
    float theta;
} Camera;

#endif
