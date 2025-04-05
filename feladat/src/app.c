#include <stdio.h>

#include "app.h"

#include <SDL2/SDL_image.h>


void init_app(App* app, int width, int height) {
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }
    
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    width = DM.w;
    height = DM.h;

    app->window = SDL_CreateWindow(
        "Game",
        0, 0,
        width, height,
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
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;

    app->flashlight_on = false;

    SDL_SetRelativeMouseMode(SDL_TRUE);

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


void reshape(GLsizei width, GLsizei height) {
    double ratio = (double)width / (double)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    double near = 0.08;
    double far = 100.0;
    double view_size = 0.08;

    glFrustum(
        -view_size * ratio, view_size * ratio,
        -view_size, view_size,
        near, far
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
                if (!app->camera.is_jumping) {
                    if (app->camera.is_crouching) {
                        app->camera.position.z += 0.8;
                    } else {
                        app->camera.position.z -= 0.8;
                    }
                    app->camera.is_crouching = !app->camera.is_crouching;
                }
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
                if (!app->camera.is_jumping) { 
                    if (app->camera.is_crouching) {
                        app->camera.position.z += 0.8;
                    }
                    app->camera.is_crouching = false;

                    app->camera.is_jumping = true;
                    app->camera.speed.z = 5;
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
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
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
                if (!app->camera.is_jumping) {
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

    update_camera(&(app->camera), elapsed_time);
    // update_scene(&(app->scene));
}


void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene), &(app->camera));
    glPopMatrix();   

    SDL_GL_SwapWindow(app->window);
}


void destroy_app(App* app) {
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
