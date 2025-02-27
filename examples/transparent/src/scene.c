#include "scene.h"

#include <GL/gl.h>

void init_scene(Scene* scene)
{
    scene->flower_texture = load_texture("assets/textures/flower.png");
    glBindTexture(GL_TEXTURE_2D, scene->flower_texture);
    scene->is_reversed = true;
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    draw_origin();
    if (scene->is_reversed) {
        for (int i = 7; i >= 0; --i) {
            glPushMatrix();
            glTranslatef(0, 0.5 * i, 0);
            draw_flower();
            glPopMatrix();
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            glPushMatrix();
            glTranslatef(0, 0.5 * i, 0);
            draw_flower();
            glPopMatrix();
        }
    }
}

void draw_origin()
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void draw_flower()
{
    glBegin(GL_QUADS);

    glColor3f(1, 1, 1);

    glTexCoord2f(1, 1);
    glVertex3f(0, 0, 0);

    glTexCoord2f(1, 0);
    glVertex3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(1, 0, 1);

    glTexCoord2f(0, 1);
    glVertex3f(1, 0, 0);

    glEnd();
}

