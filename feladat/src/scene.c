#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>


void init_scene(Scene* scene) {
    load_model(&(scene->table), "assets/models/table.obj");
    scene->texture_id = load_texture("assets/textures/table.jpg");

    //glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 1.0;
    scene->material.ambient.green = 1.0;
    scene->material.ambient.blue = 1.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 0.5;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 0.0;
}


void set_lighting() {
    float ambient_light[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    float diffuse_light[] = { 0.0f, 0.3f, 0.3f, 1.0f };
    float specular_light[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    float position[] = { 0.0f, 0.0f, 3.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}


void set_material(const Material* material) {
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


void update_scene(Scene* scene) {
}


void render_scene(const Scene* scene) {
    set_material(&(scene->material));
    set_lighting();

    glPushMatrix();  
    glTranslatef(0.0f, 0.0f, 2.0f);
    glScalef(0.008f, 0.008f, 0.008f);
    draw_model(&(scene->table));  
    glPopMatrix();
}
