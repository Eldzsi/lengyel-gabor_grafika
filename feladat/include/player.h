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


/*
Initialize the player with default values.
*/
void init_player(Player* player);

/*
Spawn the player at the starting position.
*/
void spawn_player(Player* player);

/*
Decrease player's health and respawn.
*/
void kill_player(Player* player);

/*
Set the camera view based on player position and rotation.
*/
void set_view(const Player* player);

/*
Set the forward/backward movement speed of the player.
*/
void set_camera_speed(Player* player, double speed);

/*
Set the side movement speed of the player.
*/
void set_camera_side_speed(Player* player, double speed);

/*
Rotate the camera horizontally and vertically.
*/
void rotate_camera(Player* player, double horizontal, double vertical);

/*
Update the player's position and handle collisions.
*/
void update_camera(Player* player, double time, Scene* scene);

/*
Check collision with objects in the scene at a given position.
Returns the name of the object if collision occurs, otherwise NULL.
*/
const char* check_collision(double x, double y, double z, const Scene* scene, bool crouching);

/*
Try moving the player to a given position, considering collisions.
*/
void try_move(Player* player, Scene* scene, double x, double y, double z);

/*
Make the player jump if on ground.
*/
void jump(Player* player, App* app);

/*
Toggle the flashlight on or off.
*/
void use_flashlight(Player* player);

/*
Change the brightness of the flashlight.
*/
void change_flashlight_brightness(Player* player, float value);

/*
Enable or disable sprinting, adjusting movement speed.
*/
void set_sprint_enabled(Player* player, bool enabled);

/*
Check if crouching is enabled.
*/
bool is_crouch_enabled(Player* player);

/*
Enable or disable crouching and adjust player height.
*/
void set_crouch_enabled(Player* player, bool enabled);

/*
Decrease the player's oxygen level over time.
Zero oxygen level leads to the player's death.
*/
void update_oxygen(Player* player, float elapsed_time);

/*
Get the current health of the player.
*/
int get_player_health(const Player* player);

/*
Set the health value of the player.
*/
void set_player_health(Player* player, int value);


#endif
