#include "camera.h"
#include "app.h"

#include <GL/gl.h>

#include <math.h>
#include <string.h>



void init_camera(Camera* camera, App* app) {
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

    if (app->health > 0) {
        app->health -= 1;
    }
}


void update_camera(Camera* camera, App* app, double time, Scene* scene) {
    double angle = degree_to_radian(camera->rotation.z);
    double side_angle = degree_to_radian(camera->rotation.z + 90.0);
    const float gravity = -9.8f;

    double next_x = camera->position.x + cos(angle) * camera->speed.y * time;
    double next_y = camera->position.y + sin(angle) * camera->speed.y * time;

    const char* collided = check_collision(next_x, camera->position.y, camera->position.z, scene, camera->is_crouching);
    if (!collided) {
        camera->position.x = next_x;
    } else if (strcmp(collided, "lava") == 0) {
        init_camera(camera, app);
    }

    collided = check_collision(camera->position.x, next_y, camera->position.z, scene, camera->is_crouching);
    if (!collided) {
        camera->position.y = next_y;
    } else if (strcmp(collided, "lava") == 0) {
        init_camera(camera, app);
    }


    next_x = camera->position.x + cos(side_angle) * camera->speed.x * time;
    next_y = camera->position.y + sin(side_angle) * camera->speed.x * time;

    collided = check_collision(next_x, camera->position.y, camera->position.z, scene, camera->is_crouching);

    if (!collided) {
        camera->position.x = next_x;
    } else if (strcmp(collided, "lava") == 0) {
        init_camera(camera, app);
    }

    collided = check_collision(camera->position.x, next_y, camera->position.z, scene, camera->is_crouching);
    if (!collided) {
        camera->position.y = next_y;
    } else if (strcmp(collided, "lava") == 0) {
        init_camera(camera, app);
    }

    camera->speed.z += gravity * time;

    double next_z = camera->position.z + camera->speed.z * time;

    collided = check_collision(camera->position.x, camera->position.y, next_z, scene, camera->is_crouching);
    if (!collided) {
        camera->position.z = next_z;
    } else {
        if (strcmp(collided, "lava") == 0) {
            init_camera(camera, app);
        } else {
            if (camera->speed.z < 0) {
                camera->speed.z = 0;
            }
        }
    }
}


const char* check_collision(double x, double y, double z, const Scene* scene, bool crouching) {
    if (!crouching) {
        for (int i = 0; i < scene->object_count; ++i) {
            BoundingBox box = scene->bounding_boxes[i];
            if (x >= box.min.x && x <= box.max.x &&
                y >= box.min.y && y <= box.max.y &&
                z >= box.min.z && z <= box.max.z) {
                return scene->objects[i].name;
            }
        }

        BoundingBox box = scene->floor_bounding_box;
        if (x >= box.min.x && x <= box.max.x &&
            y >= box.min.y && y <= box.max.y &&
            z >= box.min.z && z <= box.max.z) {
            return "floor";
        }
    } else {
        for (int i = 0; i < scene->object_count; ++i) {
            BoundingBox box = scene->crouch_bounding_boxes[i];
            if (x >= box.min.x && x <= box.max.x &&
                y >= box.min.y && y <= box.max.y &&
                z >= box.min.z && z <= box.max.z) {
                return scene->objects[i].name;
            }
        }

        BoundingBox box = scene->floor_crouch_bounding_box;
        if (x >= box.min.x && x <= box.max.x &&
            y >= box.min.y && y <= box.max.y &&
            z >= box.min.z && z <= box.max.z) {
            return "floor";
        }
    }

    return NULL;
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