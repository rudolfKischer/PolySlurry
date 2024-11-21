#include "include/render.h"


#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define clamp(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

struct PixelBuffer {
    int width;
    int height;
    int depth;
    unsigned char* pixels;
};

struct Mesh {
    float* v; // Vertices
    int* f; // Faces
    int nFaces;
    int nVertices;
    mat4 transform;
};

struct Line {
    vec3 p1;
    vec3 p2;
    float stroke;
};

struct Poly {
    vec3 a;
    vec3 b;
    vec3 c;
};



void testColorGradient(struct PixelBuffer* pxl_bfr) {
    for (int y = 0; y < pxl_bfr->height; y++) {
        for (int x = 0; x < pxl_bfr->width; x++) {
            int index = (y * pxl_bfr->width + x) * pxl_bfr->depth;
            pxl_bfr->pixels[index + 0] = (unsigned char)((x / (float)pxl_bfr->width) * 255);  // Red
            pxl_bfr->pixels[index + 1] = (unsigned char)((y / (float)pxl_bfr->height) * 255); // Green
            pxl_bfr->pixels[index + 2] = 128;                                        // Blue
            pxl_bfr->pixels[index + 3] = 255;                                        // Alpha
        }
    }
}

void drawCircle(struct PixelBuffer* pxl_bfr, float r, vec3 p, vec3 c) { 


    int width = pxl_bfr->width;
    int height = pxl_bfr->height;
    unsigned char* pixels = pxl_bfr->pixels;


    vec3 r_bound = {{r,r,0}};

    vec3 l_bound = a3(p, n3(r_bound));
    vec3 u_bound = a3(p, r_bound);

    int start_x = (int)(l_bound.x * width);
    int stop_x = (int)(u_bound.x * width) + 1;
    start_x = clamp(start_x, 0, width);
    stop_x = clamp(stop_x, 0, width);

    int start_y = (int)(l_bound.y * height);
    int stop_y = (int)(u_bound.y * height) + 1;
    start_y = clamp(start_y, 0, height);
    stop_y = clamp(stop_y, 0, height);

    c = s3(c, 255);

    for (int x = start_x; x < stop_x; x++) {
        for (int y = start_y; y < stop_y; y++) {
            int index = (y * width + x) * pxl_bfr->depth;
            float u = x / (float)width;
            float v = y / (float)height;
            vec3 q = {{u,v,0}};

            float d = l3(a3(q, n3(p)));

            if (d < r) {
                pixels[index + 0] = (int)c.v[0];
                pixels[index + 1] = (int)c.v[1];
                pixels[index + 2] = (int)c.v[2];
            }
        }
    }


}


void drawLine(struct PixelBuffer* pxl_bfr, struct Line line, vec3 c) {
    vec3 p1 = line.p1;
    vec3 p2 = line.p2;
    int width = pxl_bfr->width;
    int height = pxl_bfr->height;
    unsigned char* pixels = pxl_bfr->pixels;
    vec3 win_dim = {{width, height, 1.0}};

    vec3 left = p1;
    vec3 right = p2;
    vec3 top = p1;
    vec3 bottom = p2;
    if (p2.x < p1.x) {
        left = p2;
        right = p1;
    }
    if (p2.y > p1.y) {
        top = p2;
        bottom = p1;
    }
    
    
    
    vec3 start = left;
    vec3 range = {{right.x - left.x, top.y - bottom.y, 1.0 }};

    int axis = 0;
    if (range.y > range.x) {
        axis = 1;
        start = bottom;
    }
    float total_step_length = range.v[axis] ;
    vec3 step = s3(range, 1.0 / total_step_length);
    if (left.y > right.y && axis == 0) {
        step.v[1] = step.v[1] * -1.0;
    }
    if (bottom.x > top.x && axis == 1) {
        step.v[0] = step.v[0] * -1.0;
    }
    /* step = p3(step, pixel_size); */
    step = s3(step, 1.0 / win_dim.v[axis]);

    vec3 curPoint = start;


    int numSteps = (int)(total_step_length * win_dim.v[axis]);

    for (int i = 0; i < numSteps; i ++) {
        curPoint = a3(curPoint, step);

        if (curPoint.x < 0 || curPoint.y < 0) {
            continue;
        }
        if (curPoint.x > 1 || curPoint.y > 1) {
            break;
        }
        curPoint.v[2] = 0.0;

        int x = (int)(curPoint.x * width);
        int y = (int)(curPoint.y * height);

        int index = (y * pxl_bfr->width + x) * pxl_bfr->depth;

        pixels[index + 0] = c.x * 255;
        pixels[index + 1] = c.y * 255;
        pixels[index + 2] = c.z * 255;
    }  
}

void drawWirePoly(struct PixelBuffer* pxl_bfr, struct Poly poly, float stroke, vec3 color) {

    vec3 a = poly.a;
    vec3 b = poly.b;
    vec3 c = poly.c;

    struct Line AB;
    struct Line BC;
    struct Line CA;
    AB.stroke = stroke;
    BC.stroke = stroke;
    CA.stroke = stroke;
    AB.p1 = a;
    AB.p2 = b;
    BC.p1 = b;
    BC.p2 = c;
    CA.p1 = c;
    CA.p2 = a;
    drawLine(pxl_bfr, AB, color);
    drawLine(pxl_bfr, BC, color);
    drawLine(pxl_bfr, CA, color);

}

vec3 getVertice(struct Mesh mesh, int v) {
    float* vertices = mesh.v;
    int idx = 3 * v;
    vec3 vertex;
    for (int i = 0; i < 3; i++) { 
        vertex.v[i] = vertices[idx + i]; 

    };
    return vertex;
}


vec3 getPolyNormal(struct Poly poly) {
    vec3 A = poly.a;
    vec3 B = poly.b;
    vec3 C = poly.c;

    vec3 AB = a3(B, n3(A));
    vec3 AC = a3(C, n3(A));
    
    vec3 ABxAC = x3(AB, AC);

    vec3 normal = normalize3(ABxAC);
    return normal;
}

struct Poly transformPoly(struct Poly poly, mat4 transform) {
    poly.a = apply(transform, poly.a);
    poly.b = apply(transform, poly.b);
    poly.c = apply(transform, poly.c);

    return poly;
}

struct Poly getFace(struct Mesh mesh, int f) {
    int* faces = mesh.f;
    int idx = 3 * f;
    struct Poly poly;
    int v1 = faces[idx + 0];
    int v2 = faces[idx + 1];
    int v3 = faces[idx + 2];

    poly.a = getVertice(mesh, v1);
    poly.b = getVertice(mesh, v2);
    poly.c = getVertice(mesh, v3);

    return poly;
} 

vec3 getMinViewBounds(float near, float fov_deg, float aspect) {
    float fov_rad = (fov_deg / 180.0) * PI;
    float bottom = -near * tan(fov_rad * 0.5); // divde by 2 because we want the angle from midpoint
    float left = bottom * aspect;
    vec3 minp = {{left, bottom, near}};
    return minp;
}


mat4 getCamMat4(struct Camera cam) {    
    mat4 viewMat = view(cam.position, a3(cam.position,cam.viewDir), cam.up);
    return viewMat;
}

mat4 getCamProj(struct Camera cam) {

    vec3 minp = getMinViewBounds(cam.near, cam.fov, cam.aspect);
    vec3 maxp = {{-minp.v[0], -minp.v[1], cam.far}};

    /* cam.projection = ORTHOGRAPHIC; */

    mat4 projection = Im4();
    if (cam.projection == PERSPECTIVE) { projection = perspective(minp, maxp); }
    if (cam.projection == ORTHOGRAPHIC) { projection = orthographic(minp, maxp); }

    return projection;
}

int vertInBounds(vec3 v) {
    int inBounds = 1;

    for (int i = 0; i < 3; i++) {
        if (v.v[i] < -1.0) inBounds = 0;
        if (v.v[i] > 1.0) inBounds = 0;
    }

    return inBounds;
}

int polyOutOfBounds(struct Poly poly) {
    // if all of the vertices are on the wrong side of the plane, then we can cull
    vec3 minp = {{-1.0, -1.0, 0.0}};
    vec3 maxp = {{1.0, 1.0, 1.0}};
    for (int i = 0; i < 3; i++) {
        float min = minp.v[i];
        float max = maxp.v[i];
        float a = poly.a.v[i];
        float b = poly.b.v[i];
        float c = poly.b.v[i];
        if (a < min && b < min && c < min) { return 1; }
        if (a > max && b > max && c > max) { return 1; }
    }

    return 0;
}

int edgeInBounds(vec3 p0, vec3 p1) {
    vec3 minp = {{-1.0, -1.0, 0.0}};
    vec3 maxp = {{1.0, 1.0, 1.0}};


    float t_final = l3(a3(p1, n3(p0)));

    vec3 dir = normalize3(a3(p1, n3(p0)));
    vec3 p = p0; // t = 0 at point p0
    

    // get the t values for each intersection plane of the aabb
    // not that the max plane may have a negative intersection point if we are facing
    // the opposite direction relative to the axis

    vec3 tmin = p3(a3(minp, n3(p)),i3(dir)); // (minp - p) / dir 
    vec3 tmax = p3(a3(maxp, n3(p)),i3(dir));// (maxp - p) / dir
                                            //    
    // determine which intersections are closer, relative to the start
    // of the line segment at p0
    vec3 tclose = min3(tmin, tmax);  
    vec3 tfar = max3(tmin, tmax); //

    // get the closes intersecting plane and the farthest intersecting plane
    float thigh = mini3(tfar);
    float tlow = maxi3(tclose);
   
    if (thigh < tlow) { return 0; } // no intersection interval

    if (thigh < 0.0 ) { return 0; } // intersection behind line segment

    if (tlow > t_final) { return 0; } // intersection in front of line segment

    return 1;
}

int inbounds(struct Poly poly) {
    vec3 a = poly.a;
    vec3 b = poly.b;
    vec3 c = poly.c;

    if (vertInBounds(a) && vertInBounds(b) && vertInBounds(c)) { return 1; }
    // TODO: ADD CLIPPING AND EDGE BOUNDARY CHECKS
    //
    // for now just return 0 if not all vertices in frame

    return 0;


/*     int e1 = edgeInBounds(a, b); */
/*     int e2 = edgeInBounds(b, c); */
/*     int e3 = edgeInBounds(c, a); */

/*     // if all the points in the poly are on the wrongside of a plane, then out of bounds */
/*     if (polyOutOfBounds(poly)) { return 0; } */

/*     //if any of the points are behind the viewing volume dont draw */

/*     if (a.z < 0.0, b.z < 0.0, c.z < 0.0) {return 0; } */ 

/*     // if any of the edges interesect the viewing volume, then we have to draw the triangle */
/*     if (e1 || e2 || e3) { return 1; } */




/*     return 0; */

}

void drawWireMesh(struct PixelBuffer* pxl_bfr, 
        struct Mesh mesh,
        struct Camera cam,
        float stroke, 
        vec3 color) {
    
    mat4 camMat = getCamMat4(cam);
    mat4 modelMat = mesh.transform;
    mat4 projMat = getCamProj(cam);
    mat4 ndcmat = ndc();


    for (int i = 0; i < mesh.nFaces; i++) {
        struct Poly poly = getFace(mesh, i);
        poly = transformPoly(poly, modelMat);

        poly = transformPoly(poly, camMat);


        //vec3 minp = getMinViewBounds(cam.near, cam.fov, cam.aspect);
        //vec3 maxp = {{-minp.v[0], -minp.v[1], cam.far}};

        // PROJECTION MATRIX TRANSFORM ( SPECIAL CARE FOR W)

        poly = transformPoly(poly, projMat);

        int inBounds = inbounds(poly);
        if (!inBounds) { continue; }
        
        poly = transformPoly(poly, ndcmat);


        drawWirePoly(pxl_bfr, poly, stroke, color);

    }
}


void render(struct AppContext *ctx) {
    int width = ctx->window_width;
    int height = ctx->window_height;
    unsigned char* pixels = ctx->pixels;
    struct Camera cam = ctx->cam;

    struct PixelBuffer pxl_bfr = {width, height, 4, pixels}; 
    
    testColorGradient(&pxl_bfr);

    float t = ctx->frame_count * 0.001;

    float r = 0.1;

    float a = 1.0 - 2.0 * r;
    vec3 p = {{
        sin(t) * a * 0.5 + 0.5, 
        cos(t) * a * 0.5 + 0.5, 
        0.0 
    }};


    
    vec3 p0 = {{0.1, 0.1, 0}};

    struct Line line;
    line.p1 = p0;
    line.p2 = p;
    line.stroke = 0.01;

    vec3 lineColor = {{0.0,0.0,0.0}}; 
    /* struct Poly poly = { */
    /*     {{0.0,0.0,0.0}}, */
    /*     {{0.5,1.0,0.0}}, */
    /*     p */
    /* }; */

    mat4 transform = Im4();


    float cubeVertices[] = {
        // FRONT VERTICES
        1.0, 1.0, 1.0,  // top right
        -1.0, 1.0, 1.0, // top left
        1.0, -1.0, 1.0, // bottom right
        -1.0, -1.0, 1.0, // bottom left
        // BACK VERTICES
        1.0, 1.0, -1.0, // top right
        -1.0, 1.0, -1.0, // top left
        1.0, -1.0, -1.0, // bottom right
        -1.0, -1.0, -1.0, // bottom left
    };

    int cubeFaces[] = {
        // FRONT FACE
        0, 2, 3,  // Triangle 1
        0, 3, 1,  // Triangle 2

        // BACK FACE
        7, 6, 4,  // Triangle 1
        5, 7, 4,  // Triangle 2

        // LEFT FACE
        1, 3, 7,  // Triangle 1
        1, 7, 5,  // Triangle 2

        // RIGHT FACE
        4, 6, 2,  // Triangle 1
        4, 2, 0,  // Triangle 2

        // TOP FACE
        4, 0, 1,  // Triangle 1
        4, 1, 5,  // Triangle 2

        // BOTTOM FACE
        2, 6, 7,  // Triangle 1
        2, 7, 3   // Triangle 2
    };

    struct Mesh cubeMesh = {
        cubeVertices,
        cubeFaces,
        12,
        8,
        transform
    };

    /* drawWireMesh(&pxl_bfr, cubeMesh, cam, 0.1, lineColor); */
    
    int m = 3;
    float dist = -1 * m;
    float span = 0.5 * m;
    vec3 v00 = {{-span, -span, dist}}; // bottom left
    vec3 v01 = {{span, -span, dist}}; // bottom right
    vec3 v10 = {{-span, span, dist}}; // top left
    vec3 v11 = {{span, span, dist}}; // top right
    


    

    vec3 angles = {{0.0, 0.0, 0.0}};
    mat4 rotation = rotateEuler(angles);
    mat4 scale_mat = uscale(0.25);
    transform = dm4(scale_mat, transform);
    transform = dm4(rotation, transform);

    
    for (int i = 0; i < (m); i++) {
        for (int j = 0; j < (m); j++) {


            float p = (float)(i+0.5) / (float)(m);
            float q = (float)(j+0.5) / (float)(m);

            vec3 position = {{0.0, 0.0, 0.0}};
            
            position = a3(position, s3(v00,(1 - p) * (1-q)));
            position = a3(position, s3(v01,p * (1-q)));
            position = a3(position, s3(v10,(1 - p) * q));
            position = a3(position, s3(v11,p * q));

            mat4 translate_mat = translate(position);
            mat4 model_mat = dm4(translate_mat, transform);
            cubeMesh.transform = model_mat;

            drawWireMesh(&pxl_bfr, cubeMesh, cam, 0.1, lineColor);
        }
    }

    drawWireMesh(&pxl_bfr, cubeMesh, cam, 0.1, lineColor);

    glDrawPixels(width,
            height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels
            );

}
