#include <stdio.h>

#include "sound.h"


static Mix_Chunk* sound_effect = NULL;


void load_sound() {
    sound_effect = Mix_LoadWAV("assets/sounds/jump.wav");
    if (!sound_effect) {
        printf("[ERROR] Failed to load sound: %s\n", Mix_GetError());
    }
}


void play_sound() {
    if (sound_effect != NULL) {
        Mix_PlayChannel(-1, sound_effect, 0);
    }
}


void free_sound() {
    if (sound_effect != NULL) {
        Mix_FreeChunk(sound_effect);
        sound_effect = NULL;
    }
}
