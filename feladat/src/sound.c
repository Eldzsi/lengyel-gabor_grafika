#include "sound.h"

#include "stdio.h"

// #include <SDL2/SDL_mixer.h>


/* void init_mixer() {
    if (Mix_Init(MIX_INIT_MP3) == 0) {
        printf("[ERROR] SDL_mixer initialization error: %s\n", Mix_GetError());
        return;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("[ERROR] SDL_mixer audio initialization error: %s\n", Mix_GetError());
        return;
    }
}


void play_sound(const char* file_path, int loop) {
    Mix_Chunk* sound_effect = Mix_LoadWAV(file_path);
    if (!sound_effect) {
        printf("\n[ERROR] Failed to load sound: %s", Mix_GetError());
        return;
    }

    int channel = Mix_PlayChannel(-1, sound_effect, loop);
    if (channel == -1) {
        printf("[ERROR] Failed to play chunk: %s\n", Mix_GetError());
        Mix_FreeChunk(sound_effect);
        return;
    }
}


void destroy_mixer() {
    Mix_CloseAudio();
    Mix_Quit();
} */
