#include "camera.h"

#include <GL/gl.h>

#include <math.h>

#include "app.h"


void init_camera(Camera* camera) {
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 3.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;

    camera->is_crouching = false;
    camera->is_sprinting = false;
    camera->is_jumping = false;
}


void update_camera(Camera* camera, double time) {
    double angle;
    double side_angle;

    const float gravity = 18;
    const float ground_level = 0.0;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * time;
    camera->position.y += sin(angle) * camera->speed.y * time;
    camera->position.x += cos(side_angle) * camera->speed.x * time;
    camera->position.y += sin(side_angle) * camera->speed.x * time;

    if (camera->is_jumping) {
        camera->position.z += camera->speed.z * time;
    }

    double offset = (camera->is_crouching) ? 1.5 : 3.0;

    if (camera->position.z > ground_level + offset) {
        camera->speed.z -= gravity * time;
    } else {
        camera->position.z = ground_level + offset;
        camera->speed.z = 0.0;
        camera->is_jumping = false;

        if (!is_key_pressed(SDL_SCANCODE_LSHIFT)) {
            camera->is_sprinting = false;
            if (camera->speed.y > 1) camera->speed.y = 1;
            if (camera->speed.y < -1) camera->speed.y = -1;
            if (camera->speed.x > 1) camera->speed.x = 1;
            if (camera->speed.x < -1) camera->speed.x = -1;
        }
    }
}


void set_view(const Camera* camera) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}


void rotate_camera(Camera* camera, double horizontal, double vertical) {
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < -90) {
        camera->rotation.x = -90;
    }

    if (camera->rotation.x > 90.0) {
        camera->rotation.x = 90.0;
    }
}


void set_camera_speed(Camera* camera, double speed) {
    camera->speed.y = speed;
}


void set_camera_side_speed(Camera* camera, double speed) {
    camera->speed.x = speed;
}