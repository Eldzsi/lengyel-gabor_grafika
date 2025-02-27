#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene)
{
    scene->rotation_speed = 10;
    scene->angle = 0;

    load_model(&(scene->duck), "assets/models/duck.obj");
    // scene->texture_id = load_texture("assets/textures/duck.jpg");

    // glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 0.4;
    scene->material.diffuse.green = 0.8;
    scene->material.diffuse.blue = 0.8;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 100.0;
}

void set_lighting()
{
    float ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    float position[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    // float position[] = { 5.0f, 5.0f, 3.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);


    float direction[] = {-1.0, -1.0, -0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);

}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, double elapsed_time)
{
    scene->angle += scene->rotation_speed * elapsed_time;
}

void render_scene(const Scene* scene)
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glShadeModel(GL_SMOOTH);

    set_material(&(scene->material));
    set_lighting();
    draw_origin();

    glPushMatrix();
    glScalef(10, 10, 1);
    draw_plane(50);
    glPopMatrix();

    // glDisable(GL_TEXTURE_2D);
    // glRotated(scene->angle, 1, 0, 0);
    glColor3f(0.4, 1, 0);
    draw_model(&(scene->duck));
    // glEnable(GL_TEXTURE_2D);
}

void draw_origin()
{
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
}

void draw_plane(int n)
{

    glColor3f(1, 0, 0);
    glNormal3f(0, 0, 1);
    double step = 1.0 / n;
    for (int i = 0; i <= n; ++i) {
        double y = (double)i / n;
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= n; ++j) {
            double x = (double)j / n;
            glVertex3f(x, y, 0);
            glVertex3f(x, y + step, 0);
        }
        glEnd();
    }
}

