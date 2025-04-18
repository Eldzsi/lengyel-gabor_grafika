#ifndef INPUT_H
#define INPUT_H


#include "scene.h"
#include "player.h"

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdbool.h>


void handle_app_events(App* app);

SDL_bool is_key_pressed(SDL_Scancode key);


#endif
