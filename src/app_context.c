#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h> // For malloc and free
#include "include/app_context.h"
#include <math.h>

// Define window dimensions as constants
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

// Function to initialize SDL and create a window
SDL_Window* getWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Optional: Set OpenGL version and profile if needed
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create a window with OpenGL context
    SDL_Window *window = SDL_CreateWindow("OpenGL Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    return window;
}

// Function to create an OpenGL context
SDL_GLContext getGLContext(SDL_Window* window) {
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        printf("Could not create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    // Disable VSync for maximum FPS
    SDL_GL_SetSwapInterval(0);
    return gl_context;
}

void handleKeyUp(struct AppContext *ctx, const SDL_KeyboardEvent *keyEvent) {    
    switch (keyEvent->keysym.sym) {
        case SDLK_SPACE:
            ctx->ctrl.mvUp = 0;
            break;
        case SDLK_w:
            ctx->ctrl.mvForward = 0;
            break;
        case SDLK_a:
            ctx->ctrl.mvLeft = 0;
            break;
        case SDLK_s:
            ctx->ctrl.mvBack = 0;
            break;
        case SDLK_d:
            ctx->ctrl.mvRight = 0;
            break;
        case SDLK_q:
            ctx->ctrl.rotLeft = 0;
            break;
        case SDLK_e:
            ctx->ctrl.rotRight = 0;
            break;
        case SDLK_LSHIFT:
            ctx->ctrl.mvDown = 0;
            break;
        case SDLK_LCTRL:
            ctx->ctrl.sprint = 0;
            break;
        default:
            break;
    }

}




void handleKeyDown(struct AppContext *ctx, const SDL_KeyboardEvent *keyEvent) {
    
    switch (keyEvent->keysym.sym) {
        case SDLK_ESCAPE:
            ctx->running = 0; // Exit the application when Escape is pressed
            break;
        case SDLK_SPACE:
            ctx->ctrl.mvUp = 1;
            break;
        case SDLK_w:
           ctx->ctrl.mvForward = 1;
            break;
        case SDLK_a:
            ctx->ctrl.mvLeft = 1;
            break;
        case SDLK_s:
            ctx->ctrl.mvBack = 1;
            break;
        case SDLK_d:
            ctx->ctrl.mvRight = 1;
            break;
        case SDLK_q:
            ctx->ctrl.rotLeft = 1;
            break;
        case SDLK_e:
            ctx->ctrl.rotRight = 1;
            break;
        case SDLK_LSHIFT:
            ctx->ctrl.mvDown = 1;
            break;
        case SDLK_LCTRL:
            ctx->ctrl.sprint = 1;
            break;
        default:
            break;
    }

}


// Function to handle SDL events
void handleEvents(struct AppContext *ctx) {
    while (SDL_PollEvent(&ctx->event)) {
        switch (ctx->event.type) {
            case SDL_QUIT:
                ctx->running = 0;
                break;
            case SDL_KEYDOWN:
                handleKeyDown(ctx, &ctx->event.key);
                break;
            case SDL_KEYUP:
                handleKeyUp(ctx, &ctx->event.key);
                break;

            case SDL_MOUSEBUTTONDOWN:
                break;

            case SDL_MOUSEBUTTONUP:
                break;

            default:
                break;
        }
        // Handle additional events here (e.g., keyboard, mouse)
        //
        //
    }
}


// Function :to clean up resources
void uninit_app(struct AppContext *ctx) {
    if (ctx->pixels) { free(ctx->pixels); ctx->pixels = NULL; }
    if (ctx->gl_context) { SDL_GL_DeleteContext(ctx->gl_context); ctx->gl_context = NULL; }
    if (ctx->window) { SDL_DestroyWindow(ctx->window); ctx->window = NULL; }
    SDL_Quit();
}



// Function to initialize the application context
int init_app(struct AppContext *ctx) {
    ctx->window_width = WINDOW_WIDTH;
    ctx->window_height = WINDOW_HEIGHT;
    ctx->window = getWindow();
    if (!ctx->window) {
        return 1;
    }

    ctx->gl_context = getGLContext(ctx->window);
    if (!ctx->gl_context) { return 1; }

    ctx->running = 1;

    ctx->pixels = (unsigned char *)malloc(ctx->window_width * ctx->window_height * 4);
    if (!ctx->pixels) {
        printf("Failed to allocate memory for pixels.\n");
        uninit_app(ctx);
        return 1;
    }
    memset(ctx->pixels, 0, ctx->window_width * ctx->window_height * 4);


    int width = ctx->window_width;
    int height = ctx->window_height;
    float aspect = (float)width / (float)height;
    struct Camera cam = {
        {{0.0, 0.0, 0.0}},  // Cam Pos
        {{0.0, 0.0, -1.0}},  // look dir
        {{0.0, 1.0, 0.0}},  // up dir
        120,                 // FOV deg
        aspect,             // aspect ratio
        0.1,                // near clipping plane
        100.0,              // far clipping plane
        PERSPECTIVE,         // projection mode
        PI * -0.5
        };

    ctx->cam = cam;

    ctx->ctrl.mvLeft = 0;
    ctx->ctrl.mvRight = 0;
    ctx->ctrl.mvUp = 0;
    ctx->ctrl.mvDown = 0;
    ctx->ctrl.mvForward = 0;
    ctx->ctrl.mvBack = 0;
    ctx->ctrl.rotLeft = 0;
    ctx->ctrl.rotRight = 0;
    ctx->ctrl.sprint = 0;
    ctx->ctrl.speed = 0.001;





    return 0;
}
