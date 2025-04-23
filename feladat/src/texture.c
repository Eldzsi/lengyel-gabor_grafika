#include "texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>


GLuint load_texture(char* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    if (surface == NULL) {
        printf("\n[ERROR] Failed to load image: %s", IMG_GetError());
        return 0;
    }

    GLuint texture_name;
    glGenTextures(1, &texture_name);

    glBindTexture(GL_TEXTURE_2D, texture_name);

    GLenum format = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture_name;
}


void render_image(GLuint texture, float x, float y, float width, float height, int screen_width, int screen_height) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, screen_height, 0, -1, 1);

    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT);
    set_rendering_state();

    glBindTexture(GL_TEXTURE_2D, texture);
        
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + width, y);
        glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
        glTexCoord2f(0, 1); glVertex2f(x, y + height);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}


void set_rendering_state() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
