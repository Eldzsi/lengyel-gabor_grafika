#include "scene.h"
#include "bounding_box.h"
#include <utils.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <obj/load.h>
#include <obj/draw.h>


void init_scene(Scene* scene) {
    load_object_data_from_csv(scene, "objects.csv");

    scene->material.ambient.red = 0.1;
    scene->material.ambient.green = 0.1;
    scene->material.ambient.blue = 0.1;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;

    scene->floor_size = 30.0;

    scene->flashlight_intensity = 0.5;

    for (int i = 0; i < scene->object_count; i++) {
        BoundingBox box = calculate_bounding_box(&scene->objects[i], 1.0);
        scene->bounding_boxes[i] = box;

        BoundingBox c_box = calculate_bounding_box(&scene->objects[i], 0.2);
        scene->crouch_bounding_boxes[i] = c_box;
    }

    scene->floor_bounding_box.min.x = -(scene->floor_size) / 2.0f;
    scene->floor_bounding_box.max.x =  scene->floor_size / 2.0f;

    scene->floor_bounding_box.min.y = -(scene->floor_size) / 2.0f;
    scene->floor_bounding_box.max.y =  scene->floor_size / 2.0f;

    scene->floor_bounding_box.min.z = -0.1f;
    scene->floor_bounding_box.max.z = 1.1f;

    scene->floor_crouch_bounding_box.min.x = -(scene->floor_size) / 2.0f;
    scene->floor_crouch_bounding_box.max.x =  scene->floor_size / 2.0f;

    scene->floor_crouch_bounding_box.min.y = -(scene->floor_size) / 2.0f;
    scene->floor_crouch_bounding_box.max.y =  scene->floor_size / 2.0f;

    scene->floor_crouch_bounding_box.min.z = -0.1f;
    scene->floor_crouch_bounding_box.max.z = 0.3f;

    init_fog();
}


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
        // printf("\nLoaded model: %s\n", obj.model_path);
        // printf("Model size (W x D x H): %.2f x %.2f x %.2f\n", obj.size.x, obj.size.y, obj.size.z);

        scene->objects[scene->object_count++] = obj;
    }

    fclose(file);
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


void render_scene(const Scene* scene, const Camera* camera) {
    set_material(&(scene->material));
    set_lighting(camera, scene->flashlight_intensity);

    glPushMatrix();
    render_floor(scene);
    glPopMatrix();

    for (int i = 0; i < scene->object_count; i++) {
        glPushMatrix();
        glTranslatef(scene->objects[i].position.x, scene->objects[i].position.y, scene->objects[i].position.z);

        glRotatef(scene->objects[i].rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(scene->objects[i].rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(scene->objects[i].rotation.z, 0.0f, 0.0f, 1.0f);

        glScalef(scene->objects[i].scale.x, scene->objects[i].scale.y, scene->objects[i].scale.z);
        
        glBindTexture(GL_TEXTURE_2D, scene->objects[i].texture_id);

        draw_model(&scene->objects[i].model);

        glPopMatrix();
    }

    // draw_bounding_boxes(scene);
}


void render_floor(const Scene* scene) {
    float size = scene->floor_size;
    int divisions = scene->floor_size * 10;
    float step = size / divisions;
    float half = size / 2.0f;

    glNormal3f(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x0 = -half + i * step;
            float y0 = -half + j * step;
            float x1 = x0 + step;
            float y1 = y0 + step;

            glBegin(GL_QUADS);
                glVertex3f(x0, y0, 0.0f);
                glVertex3f(x1, y0, 0.0f);
                glVertex3f(x1, y1, 0.0f);
                glVertex3f(x0, y1, 0.0f);
            glEnd();
        }
    }
}


void init_fog() {
    float fog_color[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glEnable(GL_FOG);

    glFogi(GL_FOG_MODE, GL_EXP);

    glFogfv(GL_FOG_COLOR, fog_color);

    glFogf(GL_FOG_DENSITY, 0.5f);
}