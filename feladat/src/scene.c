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
  
        get_model_size(&obj.model, &obj.size.x, &obj.size.y, &obj.size.z, &obj.min, &obj.max);
        printf("\nLoaded model: %s\n", obj.model_path);
        printf("Model size (W x D x H): %.2f x %.2f x %.2f\n", obj.size.x, obj.size.y, obj.size.z);

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

    scene->flashlight_intensity = 0.5;
}


void set_lighting(const Camera* camera, float intensity) {
    float ambient_light0[] = { 0.0f * intensity, 0.0f * intensity, 0.0f * intensity, 1.0f };
    float diffuse_light0[] = { 1.0f * intensity, 1.0f * intensity, 1.0f * intensity, 1.0f };
    float specular_light0[] = { 0.0f * intensity, 0.0f * intensity, 0.0f * intensity, 1.0f };

    float position0[] = {
        camera->position.x,
        camera->position.y,
        camera->position.z,
        1.0f
    };

    float direction0[] = {
        cos(degree_to_radian(camera->rotation.z)) * cos(degree_to_radian(camera->rotation.x)),
        sin(degree_to_radian(camera->rotation.z)) * cos(degree_to_radian(camera->rotation.x)),
        sin(degree_to_radian(camera->rotation.x))
    };

    float cutoff0 = 60.0f;
    float exponent0 = 30.0f;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent0);

    float ambient_light1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuse_light1[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float specular_light1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position1[] = { 0.0f, 0.0f, 10.0f, 0.0f };

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
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
    set_lighting(camera, scene->flashlight_intensity);

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

        BoundingBox box = calculate_bounding_box(&scene->objects[i]);
        draw_bounding_box(box);

        glPopMatrix();
    }

}


void get_model_size(const Model* model, float* width, float* depth, float* height, vec3* min, vec3* max) {
    if (model->n_vertices == 0 || model->vertices == NULL) {
        *width = *depth = *height = 0.0f;
        return;
    }

    double min_x = model->vertices[0].x, max_x = model->vertices[0].x;
    double min_y = model->vertices[0].y, max_y = model->vertices[0].y;
    double min_z = model->vertices[0].z, max_z = model->vertices[0].z;

    for (int i = 1; i < model->n_vertices; i++) {
        if (model->vertices[i].x < min_x) min_x = model->vertices[i].x;
        if (model->vertices[i].x > max_x) max_x = model->vertices[i].x;

        if (model->vertices[i].y < min_y) min_y = model->vertices[i].y;
        if (model->vertices[i].y > max_y) max_y = model->vertices[i].y;

        if (model->vertices[i].z < min_z) min_z = model->vertices[i].z;
        if (model->vertices[i].z > max_z) max_z = model->vertices[i].z;
    }

    *width = (float)(max_x - min_x);
    *depth = (float)(max_y - min_y);
    *height = (float)(max_z - min_z);

    min->x = min_x;
    min->y = min_y;
    min->z = min_z;

    max->x = max_x;
    max->y = max_y;
    max->z = max_z;
}


BoundingBox calculate_bounding_box(const Object* obj) {
    BoundingBox box;

    box.min.x = obj->position.x + obj->min.x;
    box.max.x = obj->position.x + obj->max.x;

    box.min.y = obj->position.y + obj->min.y;
    box.max.y = obj->position.y + obj->max.y;

    box.min.z = obj->position.z + obj->min.z;
    box.max.z = obj->position.z + obj->max.z;

    return box;
}


void draw_bounding_box(BoundingBox box) {
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_LINES);
        glVertex3f(box.min.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.min.y, box.min.z);
        glVertex3f(box.max.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.max.y, box.min.z);
        glVertex3f(box.max.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.max.y, box.min.z);
        glVertex3f(box.min.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.min.y, box.min.z);

        glVertex3f(box.min.x, box.min.y, box.max.z); glVertex3f(box.max.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.min.y, box.max.z); glVertex3f(box.max.x, box.max.y, box.max.z);
        glVertex3f(box.max.x, box.max.y, box.max.z); glVertex3f(box.min.x, box.max.y, box.max.z);
        glVertex3f(box.min.x, box.max.y, box.max.z); glVertex3f(box.min.x, box.min.y, box.max.z);

        glVertex3f(box.min.x, box.min.y, box.min.z); glVertex3f(box.min.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.min.y, box.max.z);
        glVertex3f(box.max.x, box.max.y, box.min.z); glVertex3f(box.max.x, box.max.y, box.max.z);
        glVertex3f(box.min.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.max.y, box.max.z);
        glEnd();
    glEnable(GL_LIGHTING);
}