#include "explosion.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <stdio.h>
#include <stdlib.h>

#define GRAVITY_Z 0.1
#define MAX_LIFETIME 3

void init_explosion(Explosion* explosion)
{
  load_model(&(explosion->model), "assets/models/cube.obj");
  explosion->texture_id = load_texture("assets/textures/cube.png");

  // TODO: Implement random generator function on a range!

  for (int i = 0; i < N_PARTICLES; ++i) {
    explosion->particles[i].position.x = 0;
    explosion->particles[i].position.y = 0;
    explosion->particles[i].position.z = 0;
    explosion->particles[i].velocity.x = (double)(rand() % 2000 - 1000) / 1000;
    explosion->particles[i].velocity.y = (double)(rand() % 2000 - 1000) / 1000;
    explosion->particles[i].velocity.z = 0.5 + (double)(rand() % 1000) / 1000;
    explosion->particles[i].lifetime = (double)(rand() % 3000) / 1000;
  }
}

void update_explosion(Explosion* explosion, double elapsed_time)
{
  for (int i = 0; i < N_PARTICLES; ++i) {
    explosion->particles[i].lifetime += elapsed_time;
    if (explosion->particles[i].lifetime >= MAX_LIFETIME) {
      explosion->particles[i].position.x = 0;
      explosion->particles[i].position.y = 0;
      explosion->particles[i].position.z = 0;
      explosion->particles[i].velocity.x = (double)(rand() % 2000 - 1000) / 1000;
      explosion->particles[i].velocity.y = (double)(rand() % 2000 - 1000) / 1000;
      explosion->particles[i].velocity.z = 0.5 + (double)(rand() % 1000) / 1000;
      explosion->particles[i].lifetime = 0;
    }
    explosion->particles[i].velocity.z += (GRAVITY_Z * elapsed_time);
    explosion->particles[i].position.x += explosion->particles[i].velocity.x * elapsed_time;
    explosion->particles[i].position.y += explosion->particles[i].velocity.y * elapsed_time;
    explosion->particles[i].position.z += explosion->particles[i].velocity.z * elapsed_time;
  }
}

void render_explosion(Explosion* explosion)
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  for (int i = 0; i < N_PARTICLES; ++i) {
    glPushMatrix();
    glTranslated(
      explosion->particles[i].position.x,
      explosion->particles[i].position.y,
      explosion->particles[i].position.z
    );
    glScaled(0.1, 0.1, 0.1);
    double lifetime = explosion->particles[i].lifetime;
    int phase = (int)(lifetime);
    double t = lifetime - phase;
    switch (phase) {
    case 0:
      glColor3f(1, 1, 1 - t);
      break;
    case 1:
      glColor3f(1, 1 - t, 0);
      break;
    case 2:
      glColor3f(1 - t, 0, 0);
      break;
    default:
      printf("ERROR: Invalid phase! (%d)\n", phase);
    }
    draw_model(&(explosion->model));
    glPopMatrix();
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
}

void destroy_explosion(Explosion* explosion)
{
  // TODO: Free the model!
  // TODO: Free the texture!
}

