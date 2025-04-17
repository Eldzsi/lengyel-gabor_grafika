#ifndef TEXTURE_H
#define TEXTURE_H


#include <GL/gl.h>


typedef GLubyte Pixel[3];


GLuint load_texture(char* filename);

void render_image(GLuint texture, float x, float y, float width, float height, int screen_width, int screen_height);

void set_rendering_state();


#endif
