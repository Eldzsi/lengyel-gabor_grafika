#ifndef CAMERA_H
#define CAMERA_H


#include "utils.h"
#include "scene.h"

#include <stdbool.h>


typedef struct App App;

typedef struct Camera {
    vec3 position;
    vec3 rotation;
    vec3 speed;
    bool is_crouching;
    bool is_sprinting;
} Camera;


void init_camera(Camera* camera);

void respawn(Camera* camera, App* app);

void set_view(const Camera* camera);

void set_camera_speed(Camera* camera, double speed);

void set_camera_side_speed(Camera* camera, double speed);

void rotate_camera(Camera* camera, double horizontal, double vertical);

void update_camera(Camera* camera, App* app, double time, Scene* scene);

const char* check_collision(double x, double y, double z, const Scene* scene, bool crouching);

void try_move(Camera* camera, App* app, Scene* scene, double x, double y, double z);


#endif
