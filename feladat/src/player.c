#include "player.h"
#include "app.h"
#include "input.h"

#include <GL/gl.h>

#include <math.h>
#include <string.h>


void init_player(Player* player) {
    spawn_player(player);
    set_player_health(player, 3);
}


void spawn_player(Player* player) {
    player->position.x = -5.0;
    player->position.y = 0.0;
    player->position.z = 50.0;

    player->rotation.x = 0.0;
    player->rotation.y = 0.0;
    player->rotation.z = 0.0;
    
    player->speed.x = 0.0;
    player->speed.y = 0.0;
    player->speed.z = 0.0;

    player->is_crouching = false;
    player->is_sprinting = false;

    player->oxygen = 1.0f;
    

    player->flashlight_on = false;
    player->flashlight_brightness = 0.5;
}


void kill_player(Player* player) {
    set_player_health(player, get_player_health(player) - 1);
    spawn_player(player);
}


void set_view(const Player* player) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(player->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(player->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-player->position.x, -player->position.y, -player->position.z);
}


void set_camera_speed(Player* player, double speed) {
    player->speed.y = speed;
}


void set_camera_side_speed(Player* player, double speed) {
    player->speed.x = speed;
}


void rotate_camera(Player* player, double horizontal, double vertical) {
    player->rotation.z += horizontal;
    player->rotation.x += vertical;

    if (player->rotation.z < 0) {
        player->rotation.z += 360;
    }

    if (player->rotation.z > 360) {
        player->rotation.z -= 360;
    }

    if (player->rotation.x < -90) {
        player->rotation.x = -90;
    }

    if (player->rotation.x > 90) {
        player->rotation.x = 90;
    }
}


void update_camera(Player* player, double time, Scene* scene) {
    double angle = degree_to_radian(player->rotation.z);
    double side_angle = degree_to_radian(player->rotation.z + 90.0);
    const float gravity = -9.8f;

    double forward_x = player->position.x + cos(angle) * player->speed.y * time;
    double forward_y = player->position.y + sin(angle) * player->speed.y * time;

    try_move(player, scene, forward_x, player->position.y, player->position.z);
    try_move(player, scene, player->position.x, forward_y, player->position.z);

    double side_x = player->position.x + cos(side_angle) * player->speed.x * time;
    double side_y = player->position.y + sin(side_angle) * player->speed.x * time;

    try_move(player, scene, side_x, player->position.y, player->position.z);
    try_move(player, scene, player->position.x, side_y, player->position.z);

    player->speed.z += gravity * time;
    double next_z = player->position.z + player->speed.z * time;
    try_move(player, scene, player->position.x, player->position.y, next_z);
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


void try_move(Player* player, Scene* scene, double x, double y, double z) {
    const char* collided = check_collision(x, y, z, scene, player->is_crouching);

    if (!collided) {
        player->position.x = x;
        player->position.y = y;
        player->position.z = z;
        return;
    }
    
    if (strcmp(collided, "lava") == 0) {
        kill_player(player);
        return;
    }
    
    if (z != player->position.z && player->speed.z < 0) {
        player->speed.z = 0;
    }
}


void jump(Player* player, App* app) {
    if (check_collision(player->position.x, player->position.y, player->position.z - 0.1, &(app->scene), player->is_crouching)) {
        player->speed.z = 4.0;
        // play_sound("assets/sounds/jump.mp3", 0);
    }
}


void use_flashlight(Player* player) {
    player->flashlight_on = !player->flashlight_on;
    if (player->flashlight_on) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }
}

void change_flashlight_brightness(Player* player, float value) {
    player->flashlight_brightness += value;
    if (player->flashlight_brightness > 1.0f) {
        player->flashlight_brightness = 1.0f;
    } else if (player->flashlight_brightness < 0.0f) {
        player->flashlight_brightness = 0.0f;
    }
}


void set_sprint_enabled(Player* player, bool enabled) {
    if (enabled) {
        if (is_key_pressed(SDL_SCANCODE_W)) {
            set_camera_speed(player, 3);
        }
        if (is_key_pressed(SDL_SCANCODE_S)) {
            set_camera_speed(player, -3);
        }
        if (is_key_pressed(SDL_SCANCODE_A)) {
            set_camera_side_speed(player, 3);
        }
        if (is_key_pressed(SDL_SCANCODE_D)) {
            set_camera_side_speed(player, -3);
        }
    } else {
        if (is_key_pressed(SDL_SCANCODE_W)) {
            set_camera_speed(player, 1);
        }
        if (is_key_pressed(SDL_SCANCODE_S)) {
            set_camera_speed(player, -1);
        }
        if (is_key_pressed(SDL_SCANCODE_A)) {
            set_camera_side_speed(player, 1);
        }
        if (is_key_pressed(SDL_SCANCODE_D)) {
            set_camera_side_speed(player, -1);
        }
    }
}


bool is_crouch_enabled(Player* player) {
    return player->is_crouching;
}


void set_crouch_enabled(Player* player, bool enabled) {
    player->is_crouching = enabled;
    if (enabled) {
        player->position.z -= 0.8;
    } else {
        player->position.z += 0.8;
    }
}


void update_oxygen(Player* player, float elapsed_time) {
    if (player->oxygen > 0.0f) {
        player->oxygen -= 0.01f * elapsed_time;
    }

    if (player->oxygen <= 0.0f) {
        kill_player(player);
    }
}


int get_player_health(const Player* player) {
    return player->health;
}


void set_player_health(Player* player, int value) {
    if (value < 0) {
        player->health = 0;
    } else {
        player->health = value;
    }
}