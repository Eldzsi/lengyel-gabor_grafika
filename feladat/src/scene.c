#include "scene.h"
#include "app.h"
#include "bounding_box.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


void init_scene(Scene* scene, App* app) {
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

    app->player.flashlight_brightness = 0.5;

    for (int i = 0; i < scene->object_count; i++) {
        BoundingBox box = calculate_bounding_box(&scene->objects[i], 1.0);
        scene->bounding_boxes[i] = box;

        BoundingBox c_box = calculate_bounding_box(&scene->objects[i], 0.2);
        scene->crouch_bounding_boxes[i] = c_box;
    }
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
        sscanf(line, "%[^,],%[^,],%[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f",
            obj.model_path, obj.texture_path, obj.name,
            &obj.position.x, &obj.position.y, &obj.position.z,
            &obj.rotation.x, &obj.rotation.y, &obj.rotation.z,
            &obj.scale.x, &obj.scale.y, &obj.scale.z);

        load_model(&obj.model, obj.model_path);
        
        obj.texture_id = load_texture(obj.texture_path);
  
        get_model_size(&obj.model, &obj.size.x, &obj.size.y, &obj.size.z, &obj.min, &obj.max);

        scene->objects[scene->object_count++] = obj;
    }

    fclose(file);
}


void set_lighting(const Player* player, float brightness) {
    float ambient_light0[] = {0.0f * brightness, 0.0f * brightness, 0.0f * brightness, 1.0f};
    float diffuse_light0[] = {1.0f * brightness, 1.0f * brightness, 1.0f * brightness, 1.0f};
    float specular_light0[] = {0.0f * brightness, 0.0f * brightness, 0.0f * brightness, 1.0f};

    for (int i = 0; i < 3; ++i) {
        ambient_light0[i] *= brightness;
        diffuse_light0[i] *= brightness;
        specular_light0[i] *= brightness;
    }

    float position0[] = {
        player->position.x,
        player->position.y,
        player->position.z,
        1.0f
    };

    float direction0[] = {
        cos(degree_to_radian(player->rotation.z)) * cos(degree_to_radian(player->rotation.x)),
        sin(degree_to_radian(player->rotation.z)) * cos(degree_to_radian(player->rotation.x)),
        sin(degree_to_radian(player->rotation.x))
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

    float ambient_light1[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float diffuse_light1[] = {0.3f, 0.3f, 0.3f, 1.0f};
    float specular_light1[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float position1[] = {0.0f, 0.0f, 10.0f, 0.0f};

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
//}


void draw_object(const Object* obj) {
    glPushMatrix();
    glTranslatef(obj->position.x, obj->position.y, obj->position.z);
    glRotatef(obj->rotation.x, 1, 0, 0);
    glRotatef(obj->rotation.y, 0, 1, 0);
    glRotatef(obj->rotation.z, 0, 0, 1);
    glScalef(obj->scale.x, obj->scale.y, obj->scale.z);
    glBindTexture(GL_TEXTURE_2D, obj->texture_id);
    draw_model(&obj->model);
    glPopMatrix();
}


void render_scene(const Scene* scene, const Player* player) {
    set_material(&(scene->material));
    set_lighting(player, player->flashlight_brightness);

    for (int i = 0; i < scene->object_count; i++) {
        draw_object(&scene->objects[i]);
    }
}
