#include "path.h"

#include <GL/gl.h>

#include <stddef.h>
#include <stdio.h>

void init_path(Path* path)
{
  path->footprints = (Footprint*)malloc(sizeof(Footprint));
  path->footprints->position.x = 0;
  path->footprints->position.y = 0;
  path->footprints->next = NULL;
}

void add_footprint_to_path(Path* path, double x, double y)
{
  Footprint* footprint = path->footprints;
  while (footprint->next != NULL) {
    footprint = footprint->next;
  }
  Footprint* new_footprint = (Footprint*)malloc(sizeof(Footprint));
  new_footprint->position.x = x;
  new_footprint->position.y = y;
  new_footprint->next = NULL;
  footprint->next = new_footprint;
}

void render_path(const Path* path)
{
  Footprint* footprint = path->footprints;
  while (footprint->next != NULL) {
    // printf("(%lf, %lf)\n", footprint->position.x, footprint->position.y);
    draw_footprint(footprint);
    footprint = footprint->next;
  }
}

void draw_footprint(const Footprint* footprint)
{
  glPushMatrix();
  glTranslatef(footprint->position.x, footprint->position.y, 0);
  glBegin(GL_QUADS);
  glColor3f(0.2, 0.2, 0.2);
  glVertex3f(-0.1, -0.1, 0);
  glVertex3f(-0.1,  0.1, 0);
  glVertex3f( 0.1,  0.1, 0);
  glVertex3f( 0.1, -0.1, 0);
  glEnd();
  glPopMatrix();
}

void destroy_path(Path* path)
{
  Footprint* footprint = path->footprints;
  while (footprint != NULL) {
    Footprint* removable = footprint;
    footprint = footprint->next;
    free(removable);
  }
}

