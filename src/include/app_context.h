#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include <SDL2/SDL.h>
#include "camera.h"



struct Controller {
    int mvLeft;
    int mvRight;
    int mvUp;
    int mvDown;
    int mvForward;
    int mvBack;
    int rotLeft;
    int rotRight;
    int sprint;
    float speed;
};

struct AppContext {
    SDL_Window *window;
    SDL_GLContext gl_context;
    SDL_Event event;
    int running;
    unsigned char *pixels;
    int window_width;
    int window_height;
    int frame_count;
    Camera cam;
    struct Controller ctrl;
};

int init_app(struct AppContext *ctx);
void uninit_app(struct AppContext *ctx);
void handleEvents(struct AppContext *ctx);

#endif
