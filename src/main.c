#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h> // For malloc and free
#include "include/render.h"   
#include <math.h>


void frameCounter(struct timespec* start, struct timespec* end) {
    clock_gettime(CLOCK_MONOTONIC, end);
    float elapsed_time = (end->tv_sec - start->tv_sec) + 
                         (end->tv_nsec - start->tv_nsec) / 1e9f;
    if (elapsed_time > 0.0f) {
        double fps = 1.0 / elapsed_time;
        printf("FPS: %.2f\n", fps);
    }
    clock_gettime(CLOCK_MONOTONIC, start);
}

void updateCameraController(struct AppContext* ctx) {
    struct Controller ctrl = ctx->ctrl;
    int l = ctrl.mvLeft;
    int r = ctrl.mvRight;
    int u = ctrl.mvUp;
    int d = ctrl.mvDown;
    int f = ctrl.mvForward;
    int b = ctrl.mvBack;
    int sprint = ctrl.sprint;

    int rl = ctrl.rotLeft;
    int rr = ctrl.rotRight;

    
    //get current rotation


    vec3 viewDir =  ctx->cam.viewDir;

    viewDir.v[1] = 0;
    viewDir = normalize3(viewDir);

    float theta = ctx->cam.theta;
    


    float xRot = 0.0;
    if (rl == 1) { xRot += 1.0; }
    if (rr == 1) { xRot -= 1.0; }

    xRot *= 0.001;


    



    theta = theta + xRot;

    ctx->cam.theta = theta;

    float new_sin_theta = sin(theta);
    float new_cos_theta = cos(theta);



    vec3 newViewDir = {{new_cos_theta, 0.0, new_sin_theta}};


    ctx->cam.viewDir.v[0] = newViewDir.v[0];
    ctx->cam.viewDir.v[2] = newViewDir.v[2];


    vec3 camPos = ctx->cam.position;
    vec3 camUp = ctx->cam.up;
    mat4 viewFrameMat = viewFrame(camPos,  a3(ctx->cam.viewDir, camPos), camUp);
    viewFrameMat = t4(viewFrameMat); // get inverse to go from cam to world





    vec3 motion = {{0.0, 0.0, 0.0}};



    if (l == 1) { motion.v[0] -=1.0; }
    if (r == 1) { motion.v[0] +=1.0; }
    if (u == 1) { motion.v[1] +=1.0; }
    if (d == 1) { motion.v[1] -=1.0; }
    if (f == 1) { motion.v[2] -=1.0; }
    if (b == 1) { motion.v[2] +=1.0; }



    motion = s3(motion, ctrl.speed);

    if (sprint == 1) {
        motion = s3(motion, 10);
    }

   // convert motion to camera frame
    motion = apply(viewFrameMat, motion);

    ctx->cam.position = a3(ctx->cam.position, motion);

}


/* int main(int argc, char *argv[]) { */
int main() {
    
    

    struct AppContext ctx;

    if (init_app(&ctx)) {
        return 1;
    }

    // Initialize frame timing
    struct timespec start, end;
    frameCounter(&start, &end);



    // Main loop
    while (ctx.running) {
        
        handleEvents(&ctx);
        
        updateCameraController(&ctx);
        render(&ctx); 
        SDL_GL_SwapWindow(ctx.window);
        /* frameCounter(&start, &end); */
        ctx.frame_count++;
    }

    uninit_app(&ctx);
    return 0;
}
