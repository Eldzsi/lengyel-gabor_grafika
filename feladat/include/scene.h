#ifndef SCENE_H
#define SCENE_H

#include "bounding_box.h"
#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_OBJECTS 100

typedef struct Camera Camera;

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
} Object;

typedef struct Scene {
    Object objects[MAX_OBJECTS];
    BoundingBox bounding_boxes[MAX_OBJECTS];
    BoundingBox crouch_bounding_boxes[MAX_OBJECTS];
    BoundingBox floor_bounding_box;
    BoundingBox floor_crouch_bounding_box;
    int object_count;
    Material material;
    float flashlight_intensity;
    float floor_size;
} Scene;

void load_object_data_from_csv(Scene* scene, const char* filename);

void init_scene(Scene* scene);

void set_lighting(const Camera* camera, float intensity);

void set_material(const Material* material);

// void update_scene(Scene* scene);

void render_floor(const Scene* scene);

void render_scene(const Scene* scene, const Camera* camera);

void get_model_size(const Model* model, float* width, float* depth, float* height, vec3* min_out, vec3* max_out);

void init_fog();

#endif
