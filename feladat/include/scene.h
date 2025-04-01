#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_OBJECTS 100

typedef struct Object {
    Model model;
    GLuint texture_id;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    char model_path[128];
    char texture_path[128];
} Object;

typedef struct Scene {
    Object objects[MAX_OBJECTS];
    int object_count;
    Material material;
} Scene;

void load_object_data_from_csv(Scene* scene, const char* filename);

void init_scene(Scene* scene);

void set_lighting();

void set_material(const Material* material);

// void update_scene(Scene* scene);

void render_floor(float size);

void render_scene(const Scene* scene);

#endif
