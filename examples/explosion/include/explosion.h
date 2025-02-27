#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "texture.h"
#include "utils.h"

#include <obj/model.h>

#define N_PARTICLES 100

typedef struct Particle {
  vec3 position;
  vec3 velocity;
} Particle;

typedef struct Explosion {
  Model model;
  GLuint texture_id;
  Particle particles[N_PARTICLES];
} Explosion;

/**
 * Initialize the particles of the explosion.
 */
void init_explosion(Explosion* explosion);

/**
 * Update the position of the particles.
 */
void update_explosion(Explosion* explosion, double elapsed_time);

/**
 * Render the particles.
 */
void render_explosion(Explosion* explosion);

/**
 * Deallocate the resources.
 */
void destroy_explosion(Explosion* explosion);

#endif /* EXPLOSION_H */

