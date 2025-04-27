#include "app.h"
#include "player.h"

#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string.h>


void init_app(App* app) {
    int error_code = SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO);
    if (error_code != 0) {
        printf("\n[ERROR] SDL initialization error: %s", SDL_GetError());
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
        printf("\n[ERROR] Unable to create the application window!");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("\n[ERROR] IMG initialization error: %s", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("\n[ERROR] Unable to create the OpenGL context!");
        return;
    }

    init_opengl();
    reshape(app->width, app->height);

    init_player(&(app->player));
    init_scene(&(app->scene));

    // init_mixer();
    // play_sound("assets/sounds/lava.mp3", -1);

    app->is_running = true;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    for (int i = 0; i < 100; i++) {
        app->images[i].texture = 0;
    }

    char* image_paths[] = {
        "assets/images/heart.png", "assets/images/heart.png", "assets/images/heart.png", 
        "assets/images/dead_heart.png", "assets/images/dead_heart.png", "assets/images/dead_heart.png", 
        "assets/images/flashlight_0.png", "assets/images/flashlight_1.png", 
        "assets/images/key_f.png", "assets/images/menu.png", "assets/images/close.png",
        "assets/images/o2.png"
    };
    const float image_positions[][4] = {
        {10, 10, 64, 64}, {84, 10, 64, 64}, {158, 10, 64, 64}, 
        {10, 10, 64, 64}, {84, 10, 64, 64}, {158, 10, 64, 64}, 
        {app->width - 90, app->height - 90, 80, 80}, {app->width - 90, app->height - 90, 80, 80}, 
        {app->width - 42, app->height - 37, 24, 24}, {app->width / 2 - 250, app->height / 2 - 350 / 2, 500, 350}, 
        {app->width / 2 + 225, app->height / 2 - 350 / 2 + 5, 20, 20},
        {106, 85, 20, 20}
    };

    for (int i = 0; i < 12; i++) {
        add_image(app, image_paths[i], image_positions[i][0], image_positions[i][1], image_positions[i][2], image_positions[i][3]);
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


void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    reshape(app->width, app->height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    set_view(&(app->player));
    render_scene(&(app->scene), &(app->player));
    glPopMatrix();   

    render_oxygen(app, &(app->player));

    render_images(app);

    SDL_GL_SwapWindow(app->window);
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


void render_oxygen(App* app, Player* player) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, app->width, app->height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    float x = 10.0f;
    float y = 80.0f;
    float width = 213.0f;
    float height = 30.0f;

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();

    glColor3f(0.0f, 0.6f, 1.0f);
    float fillWidth = width * player->oxygen;
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + fillWidth, y);
        glVertex2f(x + fillWidth, y + height);
        glVertex2f(x, y + height);
    glEnd();

    glPopMatrix();
    glColor3f(1.0f, 1.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}


void update_app(App* app) {
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->player), elapsed_time, &(app->scene));
    update_scene(&(app->scene), &(app->player), elapsed_time);
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
