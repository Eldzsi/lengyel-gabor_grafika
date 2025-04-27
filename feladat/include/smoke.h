#ifndef SMOKE_H
#define SMOKE_H


#include "texture.h"
#include "utils.h"

#include <obj/model.h>


typedef struct Particle {
    vec3 position;
    vec3 velocity;
    double lifetime;
} Particle;

typedef struct Smoke {
    Model model;
    GLuint texture_id;
    vec3 position;
    Particle* particles;
    int particle_count;
    float spread_x;
    float spread_y;
    float spread_z;
} Smoke;


/*
Initializes the smoke system by creating particles around a given position.
Parameters define how many particles to create and their initial spread.
*/
void init_smoke(Smoke* smoke, vec3 position, int particle_count, float spread_x, float spread_y, float spread_z);

/*
Updates the state of all smoke particles based on the elapsed time.
Handles particle movement, gravity, and lifetime reset.
*/
void update_smoke(Smoke* smoke, double elapsed_time);

/*
Renders all smoke particles on the screen with fading transparency.
*/
void render_smoke(const Smoke* smoke);

/*
Releases the memory allocated for smoke particles.
*/
void destroy_smoke(Smoke* smoke);


#endif 

