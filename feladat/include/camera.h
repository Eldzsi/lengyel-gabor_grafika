#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include <stdbool.h>

typedef struct Camera {
    vec3 position;
    vec3 rotation;
    vec3 speed;
    
    bool is_crouching;
    bool is_sprinting;
    bool is_jumping;
} Camera;

void init_camera(Camera* camera);

void update_camera(Camera* camera, double time);

void set_view(const Camera* camera);

void rotate_camera(Camera* camera, double horizontal, double vertical);

void get_camera_speed(Camera* camera);

void set_camera_speed(Camera* camera, double speed);

void set_camera_side_speed(Camera* camera, double speed);

#endif
