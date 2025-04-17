#ifndef APP_H
#define APP_H


#include "scene.h"
#include "camera.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdbool.h>


typedef struct Scene Scene;
typedef struct Camera Camera;

typedef struct Image {
    GLuint texture; 
    float x;
    float y; 
    float width;
    float height;
    char* filename;
} Image;

typedef struct App {
    SDL_Window* window;
    int width;
    int height;
    SDL_GLContext gl_context;
    bool is_running;
    double uptime;
    Camera camera;
    Scene scene;
    bool flashlight_on;
    Image images[100];
    bool menu_open;
    int health;
} App;


void init_app(App* app);

void init_opengl();

void reshape(int width, int height);

void handle_app_events(App* app);

void update_app(App* app);

void render_app(App* app);

void destroy_app(App* app);

SDL_bool is_key_pressed(SDL_Scancode key);

void add_image(App* app, char* filename, float x, float y, float width, float height);

void render_images(App* app);


#endif
