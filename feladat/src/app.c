#include "app.h"

#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string.h>


void init_app(App* app) {
    int error_code = SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    int inited_loaders;

    app->is_running = false;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    app->menu_open = false;

    app->width = DM.w;
    app->height = DM.h;

    app->window = SDL_CreateWindow(
        "Game",
        0, 0,
        app->width, app->height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(app->width, app->height);

    init_camera(&(app->player));
    init_scene(&(app->scene), app);

    // init_mixer();
    // play_sound("assets/sounds/lava.mp3", -1);

    app->is_running = true;

    app->player.flashlight_on = false;

    app->player.health = 3;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    for (int i = 0; i < 100; i++) {
        app->images[i].texture = 0;
    }

    char* image_paths[] = {
        "assets/images/heart.png", "assets/images/heart.png", "assets/images/heart.png", 
        "assets/images/dead_heart.png", "assets/images/dead_heart.png", "assets/images/dead_heart.png", 
        "assets/images/flashlight_0.png", "assets/images/flashlight_1.png", 
        "assets/images/key_f.png", "assets/images/menu.png", "assets/images/close.png"
    };
    const float image_positions[][4] = {
        {10, 10, 64, 64}, {84, 10, 64, 64}, {158, 10, 64, 64}, 
        {10, 10, 64, 64}, {84, 10, 64, 64}, {158, 10, 64, 64}, 
        {app->width - 90, app->height - 90, 80, 80}, {app->width - 90, app->height - 90, 80, 80}, 
        {app->width - 42, app->height - 37, 24, 24}, {app->width / 2 - 250, app->height / 2 - 350 / 2, 500, 350}, 
        {app->width / 2 + 225, app->height / 2 - 350 / 2 + 5, 20, 20}
    };

    for (int i = 0; i < 11; i++) {
        add_image(app, image_paths[i], image_positions[i][0], image_positions[i][1], image_positions[i][2], image_positions[i][3]);
    }
}


void add_image(App* app, char* filename, float x, float y, float width, float height) {
    for (int i = 0; i < 100; i++) {
        if (app->images[i].texture == 0) {
            app->images[i].texture = load_texture(filename);
            app->images[i].x = x;
            app->images[i].y = y;
            app->images[i].width = width;
            app->images[i].height = height;
            app->images[i].filename = filename;
            break;
        }
    }
}


void render_images(App* app) {
    int heart_count = 0;
    GLuint dead_heart_texture = 0;

    for (int j = 0; j < 100; j++) {
        if (strcmp(app->images[j].filename, "assets/images/dead_heart.png") == 0) {
            dead_heart_texture = app->images[j].texture;
            break;
        }
    }

    for (int i = 0; i < 100; i++) {
        if (app->images[i].texture != 0) {
            if (strcmp(app->images[i].filename, "assets/images/heart.png") == 0) {
                GLuint texture_to_render = (heart_count < app->player.health) ? app->images[i].texture : dead_heart_texture;

                render_image(
                    texture_to_render,
                    app->images[i].x,
                    app->images[i].y,
                    app->images[i].width,
                    app->images[i].height,
                    app->width,
                    app->height
                );
                heart_count++;
            } else if (strcmp(app->images[i].filename, "assets/images/flashlight_0.png") == 0) {
                if (!app->player.flashlight_on) {
                    render_image(app->images[i].texture, app->images[i].x, app->images[i].y, app->images[i].width, app->images[i].height, app->width, app->height);
                }
            } else if (strcmp(app->images[i].filename, "assets/images/flashlight_1.png") == 0) {
                if (app->player.flashlight_on) {
                    render_image(app->images[i].texture, app->images[i].x, app->images[i].y, app->images[i].width, app->images[i].height, app->width, app->height);
                }
            } else if (strcmp(app->images[i].filename, "assets/images/menu.png") == 0) {
                if (app->menu_open) {
                    render_image(app->images[i].texture, app->images[i].x, app->images[i].y, app->images[i].width, app->images[i].height, app->width, app->height);
                }
            } else if (strcmp(app->images[i].filename, "assets/images/close.png") == 0) {
                if (app->menu_open) {
                    render_image(app->images[i].texture, app->images[i].x, app->images[i].y, app->images[i].width, app->images[i].height, app->width, app->height);
                }
            } else if (strcmp(app->images[i].filename, "assets/images/dead_heart.png") != 0) {
                render_image(app->images[i].texture, app->images[i].x, app->images[i].y, app->images[i].width, app->images[i].height, app->width, app->height);
            }
        }
    }
}


void init_opengl() {
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
}


void reshape(int width, int height) {
    double ratio = (double)width / (double)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glFrustum(
        -0.08 * ratio, 0.08 * ratio,
        -0.08, 0.08,
        0.08, 100.0
    );
}


SDL_bool is_key_pressed(SDL_Scancode key) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[key] != 0;
}


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
                        init_camera(&(app->player));
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


void update_app(App* app) {
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->player), elapsed_time, &(app->scene));
    // update_scene(&(app->scene));
}


void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    reshape(app->width, app->height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    set_view(&(app->player));
    render_scene(&(app->scene), &(app->player));
    glPopMatrix();   

    render_images(app);

    SDL_GL_SwapWindow(app->window);
}


void destroy_app(App* app) {
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    for (int i = 0; i < 100; i++) {
        if (app->images[i].texture != 0) { 
            glDeleteTextures(1, &(app->images[i].texture));
            app->images[i].texture = 0;
        }
    }

    // destroy_mixer();
    SDL_Quit();
}
