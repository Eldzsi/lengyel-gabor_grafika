#include "explosion.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <stdio.h>
#include <stdlib.h>

#define GRAVITY_Z -0.1
#define FRICTION 0.8

void init_explosion(Explosion* explosion)
{
  load_model(&(explosion->model), "assets/models/cube.obj");
  explosion->texture_id = load_texture("assets/textures/cube.png");

  for (int i = 0; i < N_PARTICLES; ++i) {
    explosion->particles[i].position.x = 0;
    explosion->particles[i].position.y = 0;
    explosion->particles[i].position.z = 0;
    explosion->particles[i].velocity.x = (double)(rand() % 2000 - 1000) / 1000;
    explosion->particles[i].velocity.y = (double)(rand() % 2000 - 1000) / 1000;
    explosion->particles[i].velocity.z = 0.5 + (double)(rand() % 1000) / 1000;
  }
}

void update_explosion(Explosion* explosion, double elapsed_time)
{
  for (int i = 0; i < N_PARTICLES; ++i) {
    explosion->particles[i].velocity.z += (GRAVITY_Z * elapsed_time);
    explosion->particles[i].position.x += explosion->particles[i].velocity.x * elapsed_time;
    explosion->particles[i].position.y += explosion->particles[i].velocity.y * elapsed_time;
    explosion->particles[i].position.z += explosion->particles[i].velocity.z * elapsed_time;
    if (explosion->particles[i].position.z < 0) {
      explosion->particles[i].position.z = 0;
      explosion->particles[i].velocity.z *= -FRICTION;
    }
  }
}

void render_explosion(Explosion* explosion)
{
  for (int i = 0; i < N_PARTICLES; ++i) {
    glPushMatrix();
    glTranslated(
      explosion->particles[i].position.x,
      explosion->particles[i].position.y,
      explosion->particles[i].position.z
    );
    printf("[%4d] %lf, %lf, %lf\n",
      i,
      explosion->particles[i].position.x,
      explosion->particles[i].position.y,
      explosion->particles[i].position.z
    );
    // glScaled(0.1, 0.1, 0.1);
    draw_model(&(explosion->model));
    glPopMatrix();
  }
}

void destroy_explosion(Explosion* explosion)
{
  // TODO: Free the model!
  // TODO: Free the texture!
}

