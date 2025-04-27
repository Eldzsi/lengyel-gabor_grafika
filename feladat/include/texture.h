#ifndef TEXTURE_H
#define TEXTURE_H


#include <GL/gl.h>


typedef GLubyte Pixel[3];


/*
Loads a texture from an image file and returns the texture ID.
*/
GLuint load_texture(char* filename);

/*
Renders a texture as a 2D image on the screen.
*/
void render_image(GLuint texture, float x, float y, float width, float height, int screen_width, int screen_height);

/*
Sets the OpenGL state to render 2D images, ensuring proper blending and disabling of 3D rendering features.
*/
void set_rendering_state();


#endif
