#ifndef SCENE_H
#define SCENE_H


#include "bounding_box.h"
#include "player.h"
#include "texture.h"
#include "smoke.h"

#include <obj/model.h>


#define MAX_OBJECTS 100
#define MAX_SMOKES 10


typedef struct App App;

typedef struct Player Player;

typedef struct Object {
    Model model;
    GLuint texture_id;
    vec3 size;
    vec3 min;
    vec3 max;
    vec3 position;
    vec3 original_position;
    vec3 rotation;
    vec3 scale;
    float radius;
    float speed;
    char model_path[128];
    char texture_path[128];
    char name[32];
} Object;

typedef struct Scene {
    Material material;
    Object objects[MAX_OBJECTS];
    Smoke smokes[MAX_SMOKES];
    BoundingBox bounding_boxes[MAX_OBJECTS];
    BoundingBox crouch_bounding_boxes[MAX_OBJECTS];
    int object_count;
    int smoke_count;
    float fog_density;
} Scene;


/*
Initialize the scene, load objects and effects.
*/
void init_scene(Scene* scene);

/*
Set up static and dynamic lighting.
*/
void set_lighting(const Player* player, float brightness);

/*
Apply material settings to OpenGL.
*/
void set_material(const Material* material);

/*
Initialize fog settings for the scene.
*/
void init_fog();

/*
Render the full scene including objects and effects.
*/
void render_scene(const Scene* scene, const Player* player);

/*
Draw a single object in the scene.
*/
void draw_object(const Object* obj);

/*
Update the scene, including animations and effects.
*/
void update_scene(Scene* scene, Player* player, double elapsed_time);

/*
Update the position of moving objects based on elapsed time.
*/
void update_moving_objects(Scene* scene, double elapsed_time);

/*
Update the density and appearance of fog over time.
*/
void update_fog(Scene* scene, double elapsed_time);

/*
Load object data from a CSV file into the scene.
*/
void load_object_data_from_csv(Scene* scene, const char* filename);

/*
Load smoke particle data from a CSV file into the scene.
*/
void load_smoke_data_from_csv(Scene* scene, const char* filename);


#endif
