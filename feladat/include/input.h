#ifndef INPUT_H
#define INPUT_H


#include "scene.h"
#include "player.h"

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdbool.h>


/*
Handle all SDL events related to the app, including input and menu interactions.
*/
void handle_app_events(App* app);

/*
Check if a specific keyboard key is currently pressed.
*/
SDL_bool is_key_pressed(SDL_Scancode key);


#endif
