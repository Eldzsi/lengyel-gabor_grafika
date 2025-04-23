#ifndef APP_H
#define APP_H


#include "scene.h"
#include "player.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdbool.h>


typedef struct Scene Scene;
typedef struct Player Player;

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
    SDL_GLContext gl_context;
    Scene scene;
    Image images[100];
    Player player;
    int width;
    int height;
    double uptime;
    bool is_running;
    bool menu_open;
} App;


void init_app(App* app);

void init_opengl();

void reshape(int width, int height);

void render_app(App* app);

void add_image(App* app, char* filename, float x, float y, float width, float height);

void render_images(App* app);

void render_oxygen(App* app, Player* player);

void update_app(App* app);

void destroy_app(App* app);


#endif
