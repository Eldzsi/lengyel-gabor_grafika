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

/*
Initialize the application, including OpenGL, SDL, player, and scene.
*/
void init_app(App* app);

/*
Set up basic OpenGL settings
*/
void init_opengl();

/*
Adjust the OpenGL viewport and projection matrix based on window size.
*/
void reshape(int width, int height);

/*
Render the application, including the scene and UI elements.
*/
void render_app(App* app);

/*
Add an image to the application with specified position and size.
*/
void add_image(App* app, char* filename, float x, float y, float width, float height);

/*
Render all UI images based on their current states.
*/
void render_images(App* app);

/*
Render the oxygen bar based on the player's oxygen level.
*/
void render_oxygen(App* app, Player* player);

/*
Update the application logic, player, and scene.
*/
void update_app(App* app);

/*
Destroy the application and free resources.
*/
void destroy_app(App* app);


#endif
