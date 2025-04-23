#include "scene.h"
#include "app.h"
#include "bounding_box.h"
#include "smoke.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


void init_scene(Scene* scene, App* app) {
    load_object_data_from_csv(scene, "objects.csv");

    load_smoke_data_from_csv(scene, "particles.csv");

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

    for (int i = 0; i < scene->object_count; i++) {
        BoundingBox box = calculate_bounding_box(&scene->objects[i], 1.0);
        scene->bounding_boxes[i] = box;

        BoundingBox c_box = calculate_bounding_box(&scene->objects[i], 0.2);
        scene->crouch_bounding_boxes[i] = c_box;
    }

    scene->fog_density = 0.0f;
    init_fog();
}


void load_smoke_data_from_csv(Scene* scene, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\n[ERROR] Unable to open %s", filename);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file);

    scene->smoke_count = 0;

    while (fgets(line, sizeof(line), file) && scene->smoke_count < MAX_SMOKES) {
        float x, y, z;
        int particle_count;
        float spread_x, spread_y, spread_z;

        if (sscanf(line, "%f,%f,%f,%f,%f,%f,%d", &x, &y, &z, &spread_x, &spread_y, &spread_z, &particle_count) == 7) {
            vec3 position = {x, y, z};
            init_smoke(&scene->smokes[scene->smoke_count], position, particle_count, spread_x, spread_y, spread_z);
            scene->smoke_count++;
        } 
    }

    fclose(file);
}


void load_object_data_from_csv(Scene* scene, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\n[ERROR] Unable to open objects.csv");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file);

    scene->object_count = 0;

    while (fgets(line, sizeof(line), file)) {
        Object obj;
        sscanf(line, "%[^,],%[^,],%[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
            obj.model_path, obj.texture_path, obj.name,
            &obj.position.x, &obj.position.y, &obj.position.z,
            &obj.rotation.x, &obj.rotation.y, &obj.rotation.z,
            &obj.scale.x, &obj.scale.y, &obj.scale.z,
            &obj.radius, &obj.speed
        );

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

    for (int i = 0; i < 3; i++) {
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


void update_scene(Scene* scene, Player* player, double elapsed_time) {
    scene->smokes[0].position = player->position;

    for (int i = 0; i < scene->smoke_count; i++) {
        update_smoke(&scene->smokes[i], elapsed_time);
    }

    update_moving_objects(scene, elapsed_time);

    update_fog(scene, elapsed_time);

    update_oxygen(player, elapsed_time);
}


void update_moving_objects(Scene* scene, double elapsed_time) {
    float radius;
    float speed;
    static float angles[MAX_OBJECTS] = {0.0f};

    for (int i = 0; i < scene->object_count; i++) {
        if (strcmp(scene->objects[i].name, "rock_moving") == 0) {
            radius = scene->objects[i].radius;
            speed = scene->objects[i].speed;

            angles[i] += speed * elapsed_time;
            if (angles[i] > 2 * M_PI) {
                angles[i] -= 2 * M_PI;
            }

            scene->objects[i].position.x = cos(angles[i]) * radius;
            scene->objects[i].position.y = sin(angles[i]) * radius;
        }
    }
}


void update_fog(Scene* scene, double elapsed_time) {
    if (scene->fog_density < 1.0f) {
        scene->fog_density += 0.005f * elapsed_time;
        if (scene->fog_density > 0.5f) {
            scene->fog_density = 0.5f;
        }
        glFogf(GL_FOG_DENSITY, scene->fog_density);
    }
}


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

    for (int i = 0; i < scene->smoke_count; i++) {
        render_smoke(&(scene->smokes[i]));
    }

    for (int i = 0; i < scene->object_count; i++) {
        draw_object(&scene->objects[i]);
    }
}


void init_fog() {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);

    GLfloat fog_color[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glFogfv(GL_FOG_COLOR, fog_color);

    glFogf(GL_FOG_START, 0.5f);
    glFogf(GL_FOG_END, 50.0f);
}
