#include "include/vecmath.h"
#include <stdio.h>




vec3 zero3() { vec3 r = {0}; for(int i=0;i<3;i++) r.v[i]=0.0f; return r; }
vec4 zero4() { vec4 r = {0}; for(int i=0;i<4;i++) r.v[i]=0.0f; return r; }

vec3 a3(vec3 a, vec3 b) { vec3 r; for(int i=0;i<3;i++) r.v[i]=a.v[i]+b.v[i]; return r; }
vec4 a4(vec4 a, vec4 b) { vec4 r; for(int i=0;i<4;i++) r.v[i]=a.v[i]+b.v[i]; return r; }

vec3 p3(vec3 a, vec3 b) { vec3 r; for(int i=0;i<3;i++) r.v[i]=a.v[i]*b.v[i]; return r; }
vec4 p4(vec4 a, vec4 b) { vec4 r; for(int i=0;i<4;i++) r.v[i]=a.v[i]*b.v[i]; return r; }

vec3 s3(vec3 a, float b) { vec3 r; for(int i=0;i<3;i++) r.v[i]=a.v[i]*b; return r; }
vec4 s4(vec4 a, float b) { vec4 r; for(int i=0;i<4;i++) r.v[i]=a.v[i]*b; return r; }

vec3 n3(vec3 a) { return s3(a, -1.0f); }
vec4 n4(vec4 a) { return s4(a, -1.0f); }

vec3 i3(vec3 a) { vec3 r; for(int i=0;i<3;i++) r.v[i]=1.0f/a.v[i]; return r; }
vec4 i4(vec4 a) { vec4 r; for(int i=0;i<4;i++) r.v[i]=1.0f/a.v[i]; return r; }

float d3(vec3 a, vec3 b) { float r=0.0f; for(int i=0;i<3;i++) r+=a.v[i]*b.v[i]; return r; }
float d4(vec4 a, vec4 b) { float r=0.0f; for(int i=0;i<4;i++) r+=a.v[i]*b.v[i]; return r; }

float l3(vec3 a) { float r=0.0f; for(int i=0;i<3;i++) r+=a.v[i]*a.v[i]; return sqrtf(r); }
float l4(vec4 a) { float r=0.0f; for(int i=0;i<4;i++) r+=a.v[i]*a.v[i]; return sqrtf(r); }

vec3 normalize3(vec3 a) { float m=1.0f/l3(a); return s3(a, m); }
vec4 normalize4(vec4 a) { float m=1.0f/l4(a); return s4(a, m); }

vec3 min3(vec3 a, vec3 b) {
    vec3 r = a;
    for (int i = 0; i < 3; i++) {
        if (r.v[i] > b.v[i]) { r.v[i] = b.v[i]; }
    }
    return r;
}

vec3 max3(vec3 a, vec3 b) {
    vec3 r = a;
    for (int i = 0; i < 3; i++) {
        if (r.v[i] < b.v[i]) { r.v[i] = b.v[i]; }
    }
    return r;
}

float mini3(vec3 a) {
    float r = a.v[0];
    for (int i = 0; i < 3; i++) { if (r > a.v[i]) { r = a.v[i]; } }
    return r;
}

float maxi3(vec3 a) {
    float r = a.v[0];
    for (int i = 0; i < 3; i++) { if (r < a.v[i]) { r = a.v[i]; } }
    return r;
}

vec4 v3_to_v4(vec3 a) {
    vec4 b = {{a.x, a.y, a.z, 0}};
    return b;
}

vec3 v4_to_v3(vec4 a) {
    vec3 b = {{a.x, a.y, a.z}};
    return b;
}

vec4 hmg_coord(vec3 a) {
    vec4 b = {{a.x, a.y, a.z, 1.0}};
    return b;
}


void printVec3(vec3 a) {

    printf("[ ");
    for (int j = 0; j < 3; j++) {
        printf("%.2f ", a.v[j]);
    }
    printf(" ]\n");   
}

void printMat4(mat4 a) {
    
    for (int i = 0; i < 4; i++) {
        printf("[ ");
        for (int j = 0; j < 4; j++) {
            printf("%f ", a.v[i][j]);
        }
        printf(" ]\n");   
    }
}


vec3 unhmg_coord(vec4 a) {
    vec3 b = v4_to_v3(a);
    /* if (a.w < 0.001 && a.w > -0.001) { */
    /*     a.w = 0.1; */
    /* } */
    if (a.w < 0.1 && a.w > -0.01) {
        a.w = 0.1;
    }
    if (a.w <0.01 && a.w > -0.1) {
        a.w = -0.1;
    }

    b = s3(b, 1.0 / a.w);
    return b;
}

mat4 dm4(mat4 A, mat4 B) {
    mat4 c = {0};

    int n = 4;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 4; k++) {
                c.v[i][j] += A.v[i][k] * B.v[k][j];
            }
        }
    }

    return c;
}

vec4 dm4v4(mat4 A, vec4 b) {
    vec4 c = zero4();
    int n = 4;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c.v[i] += A.v[i][j] * b.v[j];
        }
    }

    return c;
}

vec3 apply(mat4 transform, vec3 v) {
    vec4 v_hmg_coord = hmg_coord(v);
    v_hmg_coord = dm4v4(transform, v_hmg_coord);
    return unhmg_coord(v_hmg_coord);
}

mat4 skewm4(vec3 a) {
    mat4 s = {.v = {
        {0.0, -a.z, a.y, 0.0},
        {a.z, 0.0, -a.x, 0.0},
        {-a.y, a.x, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    }};
    return s;
}

vec3 x3(vec3 a, vec3 b) {

    mat4 a_x = skewm4(a);

    vec3 c = apply(a_x, b);

    return c;
}






mat4 sm4(mat4 A, float s) {
    mat4 b = A;
    int n = 4;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b.v[i][j] = A.v[i][j] * s;
        }
    }
    return b;
}

mat4 am4(mat4 A, mat4 B) {
    int n = 4;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A.v[i][j] += B.v[i][j];
        }
    }
    return A;
}

mat4 Im4() {
    mat4 c = {0};
    int n = 4;
    for (int i = 0; i < n; i++ ) { c.v[i][i] = 1.0; }
    return c;
}

mat4 t4(mat4 a) {
    mat4 r;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.v[i][j] = a.v[j][i];
        }
    }
    return r;
}

mat4 uscale(float a) {
    mat4 s = {0};

    s.v[0][0] = a;
    s.v[1][1] = a;
    s.v[2][2] = a;
    s.v[3][3] = 1.0;

    return s;
}

mat4 scale(vec3 a) {
    mat4 s = {0};
    s.v[0][0] = a.x;
    s.v[1][1] = a.y;
    s.v[2][2] = a.z;
    s.v[3][3] = 1.0;
    return s;
}

mat4 translate(vec3 a) {
    mat4 t = Im4();
    t.v[0][3] = a.x;
    t.v[1][3] = a.y;
    t.v[2][3] = a.z;
    t.v[3][3] = 1.0;
    return t;
}






mat4 rotate(vec3 u, float theta) {

    u = normalize3(u);

    mat4 u_x = skewm4(u);
    
    float c = cos(theta);
    float s = sin(theta);
    mat4 A = sm4(u_x, s);
    mat4 B = sm4(dm4(u_x, u_x), (1.0 - c));
    B.v[3][3] = 0.0;
    A.v[3][3] = 0.0;
    mat4 I = Im4();
    mat4 R = am4(am4(I,A),B);
    return R;
}

mat4 rotateEuler(vec3 eulerAngles) {
    
    mat4 R = Im4();

    for (int i = 0; i < 3; i++) {
        vec3 axis = zero3();
        axis.v[i] = 1.0;
        mat4 R_i = rotate(axis, eulerAngles.v[i]);
        R = dm4(R_i, R);
    }
    
    return R;
}




mat4 orthographic(vec3 minp, vec3 maxp) {

    // we need to take our viewing volume, and map it to 
    // the canonical viewing volume (-1, 1)
    //
    // This means scaling it to fit, and translating it by the midpoint
    //
    /*
    Projection Matrix (P) = S * T, where:

    Scaling matrix (S):
    [  2 / (right - left),           0,            0,          0  ]
    [          0,            2 / (top - bottom),   0,          0  ]
    [          0,                    0,     2 / (far - near),  0  ]
    [          0,                    0,            0,          1  ]

    Translation matrix (T):
    [  1,   0,   0,  -(left + right) / 2 ]
    [  0,   1,   0,  -(top + bottom) / 2 ]
    [  0,   0,   -1,  -(far + near)   / 2 ]
    [  0,   0,   0,             1         ]
    */
    
    //canonical
    vec3 can_minp = {{-1.0, -1.0, -1.0}};
    vec3 can_maxp = {{1.0, 1.0, 1.0}};


    vec3 view_vol_range = a3(maxp, n3(minp));
    vec3 can_vol_range = a3(can_maxp, n3(can_minp));
    vec3 inv_view_vol_range = i3(view_vol_range);
    vec3 scale_factor = p3(can_vol_range, inv_view_vol_range);

/*     printf("ortho s %f %f %f\n", scale_factor.v[0], scale_factor.v[1], scale_factor.v[2]); */


    vec3 midpoint = s3(a3(maxp, minp), 0.5);

    mat4 scale_mat = scale(scale_factor);
    mat4 translation = translate(n3(midpoint));

    mat4 ortho = Im4();
    ortho = dm4(translation, ortho);
    ortho = dm4(scale_mat, ortho);
    
    return ortho;
}

/* mat4 perspective(vec3 minp, vec3 maxp) { */
    

/*     mat4 ortho = orthographic(minp, maxp); */

/*     float near = minp.v[2]; */
/*     float far = maxp.v[2]; */
    

/*     vec3 scale_factor = {{near, near, (near + far)}}; */
/*     mat4 scale_mat = scale(scale_factor); */
/*     vec3 translation = {{0.0, 0.0, near * far}}; */
/*     mat4 translation_mat = translate(translation); */
/*     mat4 p = Im4(); */
/*     p = dm4(scale_mat, p); */

/*     p.v[2][3] = near * far; */
/*     p.v[3][2] = -1; */
/*     p.v[3][3] = 0; */

/*     p = dm4(ortho, p); */

/*     return p; */
/* } */


mat4 perspective(vec3 minp, vec3 maxp) {

    /* printf("minp %f %f %f\n", minp.v[0], minp.v[1], minp.v[2]); */
    /* printf("maxp %f %f %f\n", maxp.v[1], maxp.v[1], maxp.v[2]); */
    float l = minp.v[0];
    float r = maxp.v[0];
    float b = minp.v[1];
    float t = maxp.v[1];
    float n = minp.v[2];
    float f = maxp.v[2];
    
    mat4 p = Im4();
    
    p.v[0][0] = (2.0 * n) / (r - l);
    p.v[0][1] = 0.0;
    p.v[0][2] = (r + l) / (r - l);
    p.v[0][3] = 0.0;
    
    p.v[1][0] = 0.0;
    p.v[1][1] = (2.0 * n) / (t - b);
    p.v[1][2] = (t + b) / (t - b);
    p.v[1][3] = 0.0;
    
    p.v[2][0] = 0.0;
    p.v[2][1] = 0.0;
    p.v[2][2] = -(f + n) / (f - n);
    p.v[2][3] = -(2.0 * f * n) / (f - n);
    
    p.v[3][0] = 0.0;
    p.v[3][1] = 0.0;
    p.v[3][2] = -1.0;
    p.v[3][3] = 0.0;
    
    return p;
}

// world to cam frame
mat4 viewFrame(vec3 eye, vec3 target, vec3 up) {

    up = normalize3(up);
    vec3 lookat = a3(target, n3(eye));
    /* vec3 lookat = {{0.0, 0.0, -1.0}}; */
    vec3 c_z = normalize3(lookat); // forward
    vec3 c_x = normalize3(x3(c_z, up)); // right
    vec3 c_y = x3(c_x, c_z); // up
    mat4 view_mat_rot = Im4(); 

    for ( int i =0; i < 3; i++ ){
        view_mat_rot.v[i][0] = c_x.v[i];
        view_mat_rot.v[i][1] = c_y.v[i];
        view_mat_rot.v[i][2] = -c_z.v[i];
    }

    return view_mat_rot;
}

mat4 view(vec3 eye, vec3 target, vec3 up) {
    mat4 view = Im4();
    mat4 view_mat_rot = viewFrame(eye, target, up); 

    mat4 view_translate = translate(n3(eye));

    view = dm4(view_translate, view);

    view = dm4(view_mat_rot, view);

    return view;
}

mat4 ndc() {
    // goes from -1 1 to 0 1

    mat4 ndcmat = Im4();
    
    vec3 scale_f = {{0.5, 0.5, 1.0}};
    vec3 translation = {{0.5, 0.5, 0.0}};
    ndcmat = dm4(scale(scale_f), ndcmat);
    ndcmat = dm4(translate(translation), ndcmat);
    return ndcmat;
}
