#ifndef SCENE_H
#define SCENE_H


#include "bounding_box.h"
#include "player.h"
#include "texture.h"

#include <obj/model.h>


#define MAX_OBJECTS 100

typedef struct App App;

typedef struct Player Player;

typedef struct Object {
    Model model;
    GLuint texture_id;
    vec3 size;
    vec3 min;
    vec3 max;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    char model_path[128];
    char texture_path[128];
    char name[32];
} Object;

typedef struct Scene {
    Object objects[MAX_OBJECTS];
    BoundingBox bounding_boxes[MAX_OBJECTS];
    BoundingBox crouch_bounding_boxes[MAX_OBJECTS];
    Material material;
    int object_count;
} Scene;


void load_object_data_from_csv(Scene* scene, const char* filename);

void init_scene(Scene* scene, App* app);

void set_lighting(const Player* player, float brightness);

void set_material(const Material* material);

// void update_scene(Scene* scene);

void draw_object(const Object* obj);

void render_scene(const Scene* scene, const Player* player);


#endif
