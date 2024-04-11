#include <game.h>

#include <stoff2d.h>
#include <ecs.h>

#include <system.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    bool running;
    u32 mcTex;
} GameData;

GameData gData;

void game_update(f32 timeStep) {
    f32 camSpeed = 500.0f;
    f32 camZoom = 300.0f;
    if (s2d_keydown(S2D_KEY_O)) {
        s2d_camera_zoom(camZoom * timeStep);
    }
    if (s2d_keydown(S2D_KEY_I)) {
        s2d_camera_zoom(-camZoom * timeStep);
    }
    if (s2d_keydown(S2D_KEY_W)) {
        s2d_camera_move(0, camSpeed * timeStep);
    }
    if (s2d_keydown(S2D_KEY_A)) {
        s2d_camera_move(-camSpeed * timeStep, 0);
    }
    if (s2d_keydown(S2D_KEY_S)) {
        s2d_camera_move(0, -camSpeed * timeStep);
    }
    if (s2d_keydown(S2D_KEY_D)) {
        s2d_camera_move(camSpeed * timeStep, 0);
    }
    system_move(timeStep);
    system_render();
}

void test_entity(f32 x, f32 y, f32 vx, f32 vy) {
    u32 eID = s2d_ecs_create_entity();
    printf("creating entity with eID: %u\n", eID);
    Component position = (Component) {
        .eID = eID,
        .type = CMP_TYPE_POSITION,
        .position = (PositionComponent) {
            .position = (clmVec2) { x, y } 
        }
    };
    Component velocity = (Component) {
        .eID = eID,
        .type = CMP_TYPE_VELOCITY,
        .velocity = (VelocityComponent) {
            .velocity = (clmVec2) { vx, vy } 
        }
    };
    Component sprite = (Component) {
        .eID = eID,
        .type = CMP_TYPE_SPRITE,
        .sprite = (SpriteComponent) {
            .size = (clmVec2) { 128.0f, 128.0f },
            .texture = gData.mcTex,
            .frame = (Frame) { 0.0f, 0.0f, 1.0f, 1.0f },
            .layer = 0
        }
    };
    s2d_ecs_add_component(position);
    s2d_ecs_add_component(velocity);
    s2d_ecs_add_component(sprite);
}

void game_init() {
    if (!s2d_initialise_engine("Paralysis")) {
        fprintf(stderr, "Could not initialise stoff2d\n");
        exit(1);
    }
    s2d_set_flags(S2D_LOG_STATS);

    gData.mcTex = s2d_load_texture("mc_sprites.png");

    int n = 100;
    for (int i = 0; i < n; i++) {
        test_entity(i * 132.0f, 0.0f, i * 20, i * 20);
    }

    s2d_ecs_print_components();

    gData.running = true;
}

void game_run() {
    while (gData.running) {
        f32 timeStep = s2d_start_frame();
        if (s2d_keydown(S2D_KEY_Q)) {
            gData.running = false;
        }
        game_update(timeStep);
        s2d_end_frame();
    }
}

void game_shutdown() {
    s2d_shutdown_engine();
}
