#include <stdio.h>

#include "app.h"
#include "sound.h"

#include <SDL2/SDL_image.h>


void init_app(App* app) {
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }
    
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

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

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    init_mixer();
    play_sound("assets/sounds/lava.mp3", -1);

    app->is_running = true;

    app->flashlight_on = false;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    add_image(app, "assets/images/heart.png", 10, 10, 64, 64);
    add_image(app, "assets/images/heart.png", 84, 10, 64, 64);
    add_image(app, "assets/images/heart.png", 158, 10, 64, 64);
}


void add_image(App* app, char* filename, float x, float y, float width, float height) {
    for (int i = 0; i < 10; i++) {
        if (app->images[i].texture == 0) {
            app->images[i].texture = load_texture(filename);
            app->images[i].x = x;
            app->images[i].y = y;
            app->images[i].width = width;
            app->images[i].height = height;
            break;
        }
    }
}


void render_images(App* app) {
    for (int i = 0; i < 10; i++) {
        if (app->images[i].texture != 0) {
            render_image(app->images[i].texture, app->images[i].x, app->images[i].y, app->images[i].width, app->images[i].height, app->width, app->height);
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
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_speed(&(app->camera), 3);
                } else {
                    set_camera_speed(&(app->camera), 1);
                }
                break;
            case SDL_SCANCODE_S:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_speed(&(app->camera), -3);
                } else {
                    set_camera_speed(&(app->camera), -1);
                }
                break;
            case SDL_SCANCODE_A:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_side_speed(&(app->camera), 3);
                } else {
                    set_camera_side_speed(&(app->camera), 1);
                }
                break;
            case SDL_SCANCODE_D:
                if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                    set_camera_side_speed(&(app->camera), -3);
                } else {
                    set_camera_side_speed(&(app->camera), -1);
                }
                break;
            case SDL_SCANCODE_C:
                if (app->camera.is_crouching) {
                    app->camera.position.z += 0.8;
                } else {
                    app->camera.position.z -= 0.8;
                }
                app->camera.is_crouching = !app->camera.is_crouching;
                break;
            case SDL_SCANCODE_LSHIFT:
                if (is_key_pressed(SDL_SCANCODE_W)) {
                    set_camera_speed(&(app->camera), 3);
                }
                if (is_key_pressed(SDL_SCANCODE_S)) {
                    set_camera_speed(&(app->camera), -3);
                }
                if (is_key_pressed(SDL_SCANCODE_A)) {
                    set_camera_side_speed(&(app->camera), 3);
                }
                if (is_key_pressed(SDL_SCANCODE_D)) {
                    set_camera_side_speed(&(app->camera), -3);
                }
                break;
            case SDL_SCANCODE_SPACE:
                if (check_collision(app->camera.position.x, app->camera.position.y, app->camera.position.z - 0.1, &(app->scene), app->camera.is_crouching)) {
                    app->camera.speed.z = 4.0;
                    play_sound("assets/sounds/jump.mp3", 0);
                }
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
                            set_camera_side_speed(&(app->camera), 3);
                        } else {
                            set_camera_side_speed(&(app->camera), 1);
                        }
                    } else if (is_key_pressed(SDL_SCANCODE_S)) {
                        if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                            set_camera_side_speed(&(app->camera), -3);
                        } else {
                            set_camera_side_speed(&(app->camera), -1);
                        }
                    } else {
                        set_camera_speed(&(app->camera), 0);
                    }
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_D:
                    if (is_key_pressed(SDL_SCANCODE_A)) {
                        if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                            set_camera_side_speed(&(app->camera), 3);
                        } else {
                            set_camera_side_speed(&(app->camera), 1);
                        }
                    } else if (is_key_pressed(SDL_SCANCODE_D)) {
                        if (is_key_pressed(SDL_SCANCODE_LSHIFT)) {
                            set_camera_side_speed(&(app->camera), -3);
                        } else {
                            set_camera_side_speed(&(app->camera), -1);
                        }
                    } else {
                        set_camera_side_speed(&(app->camera), 0);
                    }
                    break;
            case SDL_SCANCODE_F:
                app->flashlight_on = !app->flashlight_on;
                if (app->flashlight_on) {
                    glEnable(GL_LIGHT0);
                } else {
                    glDisable(GL_LIGHT0);
                }
                break;
            case SDL_SCANCODE_LSHIFT:
                if (is_key_pressed(SDL_SCANCODE_W)) {
                    set_camera_speed(&(app->camera), 1);
                }
                if (is_key_pressed(SDL_SCANCODE_S)) {
                    set_camera_speed(&(app->camera), -1);
                }
                if (is_key_pressed(SDL_SCANCODE_A)) {
                    set_camera_side_speed(&(app->camera), 1);
                }
                if (is_key_pressed(SDL_SCANCODE_D)) {
                    set_camera_side_speed(&(app->camera), -1);
                }
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetRelativeMouseState(&rel_x, &rel_y);
            rotate_camera(&(app->camera), -rel_x, -rel_y);
            break;
            case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                app->scene.flashlight_intensity += 0.1f;
                if (app->scene.flashlight_intensity > 1.0f) {
                    app->scene.flashlight_intensity = 1.0f;
                }
            } else if (event.wheel.y < 0) {
                app->scene.flashlight_intensity -= 0.1f;
                if (app->scene.flashlight_intensity < 0.0f) {
                    app->scene.flashlight_intensity = 0.0f;
                }
            }
            break;
        case SDL_QUIT:
            app->is_running = false;
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

    update_camera(&(app->camera), elapsed_time, &(app->scene));
    // update_scene(&(app->scene));
}


void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    reshape(app->width, app->height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene), &(app->camera));
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

    for (int i = 0; i < 10; i++) {
        if (app->images[i].texture != 0) { 
            glDeleteTextures(1, &(app->images[i].texture));
            app->images[i].texture = 0;
        }
    }

    destroy_mixer();
    SDL_Quit();
}
