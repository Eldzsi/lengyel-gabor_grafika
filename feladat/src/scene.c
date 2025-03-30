#include "scene.h"

#include <string.h>

#include <obj/load.h>
#include <obj/draw.h>


void init_scene(Scene* scene) {
    const char* model_paths[] = {
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj",
        "assets/models/table.obj"
    };

    const char* texture_paths[] = {
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg",
        "assets/textures/table.jpg"
    };

    vec3 positions[] = {
        {0.0f, 0.0f, 2.0f},
        {1.5f, 0.0f, 2.0f},
        {-1.5f, 0.0f, 2.0f},
        {0.0f, -2.5f, 2.0f},
        {1.5f, -2.5f, 2.0f},
        {-1.5f, -2.5f, 2.0f},
        {0.0f, 2.5f, 2.0f},
        {1.5f, 2.5f, 2.0f},
        {-1.5f, 2.5f, 2.0f},
    };

    vec3 scales[] = {
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f},
        {0.008f, 0.008f, 0.008f}
    };

    scene->object_count = sizeof(positions) / sizeof(positions[0]);

    for (int i = 0; i < scene->object_count; i++) {
        strncpy(scene->objects[i].model_path, model_paths[i], sizeof(scene->objects[i].model_path));
        strncpy(scene->objects[i].texture_path, texture_paths[i], sizeof(scene->objects[i].texture_path));

        load_model(&scene->objects[i].model, scene->objects[i].model_path);
        scene->objects[i].texture_id = load_texture(scene->objects[i].texture_path);
        
        scene->objects[i].position = positions[i];
        scene->objects[i].scale = scales[i];
    }

    scene->material.ambient.red = 0.1;
    scene->material.ambient.green = 0.1;
    scene->material.ambient.blue = 0.1;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}


void set_lighting() {
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 6.0f, 10.0f, 1.0f };
    // float direction[] = { 0.0f, -1.0f, 0.0f };
    // float cutoff = 40.0f;
    // float exponent = 20.0f

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    // glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    // glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
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


// void update_scene(Scene* scene) {
// }


void render_floor(float size) {
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-size / 2, -size / 2, 0);
        glVertex3f(size / 2, -size / 2, 0);
        glVertex3f(size / 2, size / 2, 0);
        glVertex3f(-size / 2, size / 2, 0);
    glEnd();
}


void render_scene(const Scene* scene) {
    set_material(&(scene->material));
    set_lighting();

    glPushMatrix();
    glScalef(10, 10, 1);
    render_floor(50);
    glPopMatrix();

    for (int i = 0; i < scene->object_count; i++) {
        glPushMatrix();
        glTranslatef(scene->objects[i].position.x, scene->objects[i].position.y, scene->objects[i].position.z);
        glScalef(scene->objects[i].scale.x, scene->objects[i].scale.y, scene->objects[i].scale.z);
        draw_model(&scene->objects[i].model);
        glPopMatrix();
    }
}
