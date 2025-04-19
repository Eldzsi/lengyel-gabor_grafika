#include "smoke.h"
#include "utils.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <stdio.h>
#include <stdlib.h>


#define GRAVITY_Z 0.1
#define MAX_LIFETIME 5


void init_smoke(Smoke* smoke, vec3 position, int particle_count, float spread_x, float spread_y, float spread_z) {
    smoke->position = position;
    smoke->particle_count = particle_count;
    smoke->spread_x = spread_x;
    smoke->spread_y = spread_y;
    smoke->spread_z = spread_z;

    smoke->particles = (Particle*)malloc(sizeof(Particle) * particle_count);

    load_model(&(smoke->model), "assets/models/cube.obj");

    for (int i = 0; i < particle_count; i++) {
        smoke->particles[i].position.x = position.x + generate_random_number(-spread_x, spread_x);
        smoke->particles[i].position.y = position.y + generate_random_number(-spread_y, spread_y);
        smoke->particles[i].position.z = position.z + generate_random_number(-spread_z, 0.0);

        smoke->particles[i].velocity.x = generate_random_number(-0.5, 0.5);
        smoke->particles[i].velocity.y = generate_random_number(-0.5, 0.5);
        smoke->particles[i].velocity.z = generate_random_number(0.5, 1.0);

        smoke->particles[i].lifetime = (double)(rand() % 4000) / 1000.0;
    }
}


void update_smoke(Smoke* smoke, double elapsed_time) {
    for (int i = 0; i < smoke->particle_count; i++) {
        if (smoke->particles[i].lifetime >= MAX_LIFETIME) {
            smoke->particles[i].position.x = smoke->position.x + generate_random_number(-smoke->spread_x, smoke->spread_x);
            smoke->particles[i].position.y = smoke->position.y + generate_random_number(-smoke->spread_y, smoke->spread_y);
            smoke->particles[i].position.z = smoke->position.z + generate_random_number(-smoke->spread_z, 0.0);

            smoke->particles[i].velocity.x = generate_random_number(-0.5, 0.5);
            smoke->particles[i].velocity.y = generate_random_number(-0.5, 0.5);
            smoke->particles[i].velocity.z = generate_random_number(0.5, 1.0);

            smoke->particles[i].lifetime = 0.0;
        } else {
            smoke->particles[i].lifetime += elapsed_time;
        }

        smoke->particles[i].velocity.z += (GRAVITY_Z * elapsed_time);
        smoke->particles[i].position.x += smoke->particles[i].velocity.x * elapsed_time;
        smoke->particles[i].position.y += smoke->particles[i].velocity.y * elapsed_time;
        smoke->particles[i].position.z += smoke->particles[i].velocity.z * elapsed_time;
    }
}


void render_smoke(const Smoke* smoke) {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND); 

    for (int i = 0; i < smoke->particle_count; ++i) {
        glPushMatrix();
        glTranslated(
            smoke->particles[i].position.x,
            smoke->particles[i].position.y,
            smoke->particles[i].position.z
        );
        glScaled(0.04, 0.04, 0.04);
        double alpha = 1.0 - (smoke->particles[i].lifetime / MAX_LIFETIME);
        glColor4f(0.3f, 0.3f, 0.3f, alpha);
        draw_model(&(smoke->model));
        glPopMatrix();
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND); 
}


void destroy_smoke(Smoke* smoke) {
    if (smoke->particles != NULL) {
        free(smoke->particles);
        smoke->particles = NULL;
    }
}