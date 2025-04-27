#include "input.h"
#include "app.h"


void handle_app_events(App* app) {
    SDL_Event event;
    static int rel_x, rel_y;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            app->menu_open = !app->menu_open;

            if (app->menu_open) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_ShowCursor(SDL_ENABLE);
            } else {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                SDL_ShowCursor(SDL_DISABLE);
            }
            continue;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;
        
                if (app->menu_open) {
                    int x, y, width, height;
                    x = app->width/2 + 225;
                    y = app->height/2 - 350/2 + 5;
                    width = height = 20;

                    if (mouse_x >= x && 
                        mouse_x <= x + width && 
                        mouse_y >= y && 
                        mouse_y <= y + height) {
                        app->menu_open = !app->menu_open;
                
                        if (app->menu_open) {
                            SDL_SetRelativeMouseMode(SDL_FALSE);
                            SDL_ShowCursor(SDL_ENABLE);
                        } else {
                            SDL_SetRelativeMouseMode(SDL_TRUE);
                            SDL_ShowCursor(SDL_DISABLE);
                        }
                    }

                    x = app->width/2 - 250 + 10 + 251;
                    y = app->height/2 - 350/2 + 316;
                    width = 235;
                    height = 26;

                    if (mouse_x >= x && 
                        mouse_x <= x + width && 
                        mouse_y >= y && 
                        mouse_y <= y + height) {      
                            
                        app->is_running = false;
                    }

                    x = app->width/2 - 250 + 10;
                    y = app->height/2 - 350/2 + 316;
                    width = 235;
                    height = 26;

                    if (mouse_x >= x && 
                        mouse_x <= x + width && 
                        mouse_y >= y && 
                        mouse_y <= y + height) {                
                        init_player(&(app->player));
                    }
                }
            }
            continue;
        }

        if (event.type == SDL_QUIT) {
            app->is_running = false;
            continue;
        }

        if (app->menu_open) {
            continue;
        }

        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_speed(&(app->player), 3);
                } else {
                    set_camera_speed(&(app->player), 1);
                }
                break;
            case SDL_SCANCODE_S:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_speed(&(app->player), -3);
                } else {
                    set_camera_speed(&(app->player), -1);
                }
                break;
            case SDL_SCANCODE_A:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_side_speed(&(app->player), 3);
                } else {
                    set_camera_side_speed(&(app->player), 1);
                }
                break;
            case SDL_SCANCODE_D:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_side_speed(&(app->player), -3);
                } else {
                    set_camera_side_speed(&(app->player), -1);
                }
                break;
            case SDL_SCANCODE_C:
                set_crouch_enabled(&(app->player), !is_crouch_enabled(&(app->player)));
                break;
            case SDL_SCANCODE_LSHIFT:
                set_sprint_enabled(&(app->player), true);
                break;
            case SDL_SCANCODE_SPACE:
                jump(&(app->player), app);
                break;
            case SDL_SCANCODE_F:
                use_flashlight(&(app->player));
                break;
            case SDL_SCANCODE_E:
                change_flashlight_brightness(&(app->player), 0.1);
                break;
            case SDL_SCANCODE_Q:
                change_flashlight_brightness(&(app->player), -0.1);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                if (is_key_pressed(SDL_SCANCODE_W)) {
                    if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                        set_camera_side_speed(&(app->player), 3);
                    } else {
                        set_camera_side_speed(&(app->player), 1);
                    }
                } else if (is_key_pressed(SDL_SCANCODE_S)) {
                    if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                        set_camera_side_speed(&(app->player), -3);
                    } else {
                        set_camera_side_speed(&(app->player), -1);
                    }
                } else {
                    set_camera_speed(&(app->player), 0);
                }
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                if (is_key_pressed(SDL_SCANCODE_A)) {
                    if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                        set_camera_side_speed(&(app->player), 3);
                    } else {
                        set_camera_side_speed(&(app->player), 1);
                    }
                } else if (is_key_pressed(SDL_SCANCODE_D)) {
                    if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                        set_camera_side_speed(&(app->player), -3);
                    } else {
                        set_camera_side_speed(&(app->player), -1);
                    }
                } else {
                    set_camera_side_speed(&(app->player), 0);
                }
                break;
            case SDL_SCANCODE_LSHIFT:
                set_sprint_enabled(&(app->player), false);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetRelativeMouseState(&rel_x, &rel_y);
            rotate_camera(&(app->player), -rel_x, -rel_y);
            break;
        default:
            break;
        }
    }
}


SDL_bool is_key_pressed(SDL_Scancode key) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[key] != 0;
}