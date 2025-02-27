#ifndef SCENE_H
#define SCENE_H

#include "camera.h"

#include <GL/gl.h>

typedef struct Scene
{
  GLuint flower_texture;
  bool is_reversed;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

/**
 * Draw a flower from a texture.
 */
void draw_flower();

#endif /* SCENE_H */
