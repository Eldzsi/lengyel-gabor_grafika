#ifndef PLAYER_H
#define PLAYER_H


#include "utils.h"
#include "scene.h"

#include <stdbool.h>


typedef struct App App;

typedef struct Player {
    vec3 position;
    vec3 rotation;
    vec3 speed;
    int health;
    float oxygen;
    float flashlight_brightness;
    bool flashlight_on;
    bool is_crouching;
    bool is_sprinting;
} Player;


void init_player(Player* player);

void spawn_player(Player* player);

void kill_player(Player* player);

void set_view(const Player* player);

void set_camera_speed(Player* player, double speed);

void set_camera_side_speed(Player* player, double speed);

void rotate_camera(Player* player, double horizontal, double vertical);

void update_camera(Player* player, double time, Scene* scene);

const char* check_collision(double x, double y, double z, const Scene* scene, bool crouching);

void try_move(Player* player, Scene* scene, double x, double y, double z);

void jump(Player* player, App* app);

void use_flashlight(Player* player);

void change_flashlight_brightness(Player* player, float value);

void set_sprint_enabled(Player* player, bool enabled);

bool is_crouch_enabled(Player* player);

void set_crouch_enabled(Player* player, bool enabled);

void update_oxygen(Player* player, float elapsed_time);

int get_player_health(const Player* player);

void set_player_health(Player* player, int value);


#endif
