#include "scene.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <obj/load.h>
#include <obj/draw.h>


void load_object_data_from_csv(Scene* scene, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[Error] Unable to open objects.csv");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file);

    scene->object_count = 0;

    while (fgets(line, sizeof(line), file)) {
        Object obj;

        sscanf(line, "%[^,],%[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f",
            obj.model_path, obj.texture_path,
            &obj.position.x, &obj.position.y, &obj.position.z,
            &obj.rotation.x, &obj.rotation.y, &obj.rotation.z,
            &obj.scale.x, &obj.scale.y, &obj.scale.z);

        load_model(&obj.model, obj.model_path);
        obj.texture_id = load_texture(obj.texture_path);

        scene->objects[scene->object_count++] = obj;
    }

    fclose(file);
}


void init_scene(Scene* scene) {
    load_object_data_from_csv(scene, "objects.csv");

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


void set_lighting(const Camera* camera) {
    float ambient_light[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular_light[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    float position[] = {
        camera->position.x,
        camera->position.y,
        camera->position.z,
        1.0f
    };

    float direction[] = {
        cos(degree_to_radian(camera->rotation.z)) * cos(degree_to_radian(camera->rotation.x)),
        sin(degree_to_radian(camera->rotation.z)) * cos(degree_to_radian(camera->rotation.x)),
        sin(degree_to_radian(camera->rotation.x))
    };

    float cutoff = 60.0f;
    float exponent = 30.0f;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
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


void render_scene(const Scene* scene, const Camera* camera) {
    set_material(&(scene->material));
    set_lighting(camera);

    glPushMatrix();
    glScalef(10, 10, 1);
    render_floor(50);
    glPopMatrix();

    for (int i = 0; i < scene->object_count; i++) {
        glPushMatrix();
        glTranslatef(scene->objects[i].position.x, scene->objects[i].position.y, scene->objects[i].position.z);

        glRotatef(scene->objects[i].rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(scene->objects[i].rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(scene->objects[i].rotation.z, 0.0f, 0.0f, 1.0f);

        glScalef(scene->objects[i].scale.x, scene->objects[i].scale.y, scene->objects[i].scale.z);
        
        draw_model(&scene->objects[i].model);
        glPopMatrix();
    }
}
