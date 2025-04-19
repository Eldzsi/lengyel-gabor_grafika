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


void init_smoke(Smoke* smoke, vec3 position, int particle_count, float spread_x, float spread_y, float spread_z);

void update_smoke(Smoke* smoke, double elapsed_time);

void render_smoke(const Smoke* smoke);

void destroy_smoke(Smoke* smoke);


#endif 

