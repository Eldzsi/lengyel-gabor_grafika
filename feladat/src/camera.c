#include "camera.h"
#include "app.h"

#include <GL/gl.h>

#include <math.h>
#include <string.h>


void init_camera(Camera* camera) {
    camera->position.x = -5.0;
    camera->position.y = 0.0;
    camera->position.z = 20.0;

    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;

    camera->is_crouching = false;
    camera->is_sprinting = false;
}


void respawn(Camera* camera, App* app) {
    init_camera(camera);

    if (app->health > 0) {
        app->health -= 1;
    }
}


void set_view(const Camera* camera) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}


void set_camera_speed(Camera* camera, double speed) {
    camera->speed.y = speed;
}


void set_camera_side_speed(Camera* camera, double speed) {
    camera->speed.x = speed;
}


void rotate_camera(Camera* camera, double horizontal, double vertical) {
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360;
    }

    if (camera->rotation.z > 360) {
        camera->rotation.z -= 360;
    }

    if (camera->rotation.x < -90) {
        camera->rotation.x = -90;
    }

    if (camera->rotation.x > 90) {
        camera->rotation.x = 90;
    }
}


void update_camera(Camera* camera, App* app, double time, Scene* scene) {
    double angle = degree_to_radian(camera->rotation.z);
    double side_angle = degree_to_radian(camera->rotation.z + 90.0);
    const float gravity = -9.8f;

    double forward_x = camera->position.x + cos(angle) * camera->speed.y * time;
    double forward_y = camera->position.y + sin(angle) * camera->speed.y * time;

    try_move(camera, app, scene, forward_x, camera->position.y, camera->position.z);
    try_move(camera, app, scene, camera->position.x, forward_y, camera->position.z);

    double side_x = camera->position.x + cos(side_angle) * camera->speed.x * time;
    double side_y = camera->position.y + sin(side_angle) * camera->speed.x * time;

    try_move(camera, app, scene, side_x, camera->position.y, camera->position.z);
    try_move(camera, app, scene, camera->position.x, side_y, camera->position.z);

    camera->speed.z += gravity * time;
    double next_z = camera->position.z + camera->speed.z * time;
    try_move(camera, app, scene, camera->position.x, camera->position.y, next_z);
}


const char* check_collision(double x, double y, double z, const Scene* scene, bool crouching) {
    const BoundingBox* boxes = crouching ? scene->crouch_bounding_boxes : scene->bounding_boxes;

    for (int i = 0; i < scene->object_count; ++i) {
        BoundingBox box = boxes[i];
        if (x >= box.min.x && x <= box.max.x &&
            y >= box.min.y && y <= box.max.y &&
            z >= box.min.z && z <= box.max.z) {
            return scene->objects[i].name;
        }
    }

    return NULL;
}


void try_move(Camera* camera, App* app, Scene* scene, double x, double y, double z) {
    const char* collided = check_collision(x, y, z, scene, camera->is_crouching);

    if (!collided) {
        camera->position.x = x;
        camera->position.y = y;
        camera->position.z = z;
        return;
    }
    
    if (strcmp(collided, "lava") == 0) {
        respawn(camera, app);
        return;
    }
    
    if (z != camera->position.z && camera->speed.z < 0) {
        camera->speed.z = 0;
    }
}
